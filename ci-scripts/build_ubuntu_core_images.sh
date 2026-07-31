#!/bin/bash
# SPDX-License-Identifier: MIT

# *   Description:
# **   The Core Network Functions are built parallely.
# **   Each NF is cloned from GitHub, checked out at a specific commit,
# **   associated with a shared 'common-src' submodule commit, and then
# **   built into a Docker image.
# **   CPU cores are allocated per NF to enable efficient parallel builds.

# *   Inputs:
# **    <nfJson-file>
# ***     Path to a JSON file that maps Core Network Function (NF) names
# ***     to their corresponding Git commits or branches and common-src commits/branches to be built.


# **    <archives>
# ***     Directory where build logs and the list of failed
# ***     Network Function names will be stored.

# *   Variables:
# **   NF_JSON_FILE              - Path to the JSON file mapping NF names to
# **                               their corresponding Git commits or branches
# **   COMMON_SRC_MAP            - Associative array mapping NF names to their
# **                               specific common-src commits from the JSON file
# **   LOG_ARCHIVES              - Directory where build logs
# **                               and failure reports are stored
# **   BASE_URL                  - Base URL for cloning NF repos (override: NF_GIT_BASE_URL)
# **   CLONE_DIR                 - Workspace dir for NF clones (fresh-cloned per build)
# **   FAILED_NFS_FILE           - File containing the list of NFs whose builds failed (under LOG_ARCHIVES)
# **   FULL_FAILED_NFS_PATH      - Full path to the failed NFs file, used for logging and final check
# **   CORES_PER_BUILD           - Number of CPU cores allocated per NF build
# **   MAX_PARALLEL              - Maximum number of NF builds executed in parallel
# **   NUMA1_FIRST_HALF_START    - Starting CPU core ID for the first NUMA block
# **   NUMA1_SECOND_HALF_START   - Starting CPU core ID for the second NUMA block
# **   NUMA1_FIRST_IDX           - Index counter for CPU allocation in the first NUMA block
# **   NUMA1_SECOND_IDX          - Index counter for CPU allocation in the second NUMA block
# **   jobs_running              - Number of NF build jobs currently running in parallel
# **   BATCH_PIDS                - Array of background PIDs for a batch of parallel NF Builds
# **   NF_MAP                    - Associative array mapping NF names to NF commits/branches
# **   NF_SHA                    - Short SHA of the NF repository commit being built
# **   COMMON_SRC_SHA            - Short SHA of the common-src submodule for the NF
# **   IMAGE_TAG                 - Docker image tag for the NF, includes NF_SHA and COMMON_SRC_SHA

# *   Functionality:
# **    Validate the input arguments.
# **    Initialize the core network function directories.
# **    Parse the NF Json file.
# **    Build NFs in parallel using the network functions commits
# **    and common-src submodule commits  provided in the JSON file.
# **    When the number of running jobs reach MAX_PARALLEL,
# **    then the script waits for all builds in current batch to finish via wait_for_batch().
# **    wait_for_batch() waits for all PIDs in the batch; failed NFs are recorded
# **    in $FAILED_NFS_FILE, but the run continues so every NF is built.
# **    This script launches NF Builds in batches of MAX_PARALLEL.
# ***      Example: Batch 2 (udm, smf, upf), smf fails → smf recorded in
# ***      $FAILED_NFS_FILE; udm & upf finish and Batch 3 (nrf, udr, ausf) still runs.
# ***      After all batches finish, the script exits non-zero if any NF failed.

set -e

log() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [$1] $2"
}

log "INFO" "Script started at $(date)"

if [ $# -ne 2 ]; then
    echo "Usage: $0 <nfJson-file> <archives>"
    echo "Example: ./build_ubuntu_core_images.sh nf_map.json archives"
    exit 1
fi


NF_JSON_FILE=$1
LOG_ARCHIVES=$2

# NF repo base URL; override via NF_GIT_BASE_URL. Clones from "${BASE_URL}-<nf>.git".
BASE_URL="${NF_GIT_BASE_URL:-https://github.com/openairinterface/oai-cn5g}"
REGISTRY="${PRIVATE_REGISTRY:-}"
CLONE_DIR="$PWD/oai-cn5g-repos"
mkdir -p "$CLONE_DIR"
mkdir -p "$LOG_ARCHIVES"

# Failed NFs file
FAILED_NFS_FILE="$LOG_ARCHIVES/failed_network_functions.txt"
touch "$FAILED_NFS_FILE"
FULL_FAILED_NFS_PATH=$(realpath "$FAILED_NFS_FILE")

# Now you can use the variable anywhere
echo "Failed NFs file: $FULL_FAILED_NFS_PATH"

# --- CPU / parallel build settings ---
CORES_PER_BUILD=5 # each NF
MAX_PARALLEL=3 # Limits the max parallel docker builds
NUMA1_FIRST_HALF_START=16 # starting CPU ID
NUMA1_SECOND_HALF_START=48 # starting CPU ID
NUMA1_FIRST_IDX=0 # index/counter for NUMA
NUMA1_SECOND_IDX=0 # index/counter for NUMA
jobs_running=0 # How many builds are running in the background
BATCH_PIDS=()   # PIDs of all NF builds in the current batch

# --- Parse NF JSON into associative array ---
declare -A NF_MAP
declare -A COMMON_SRC_MAP

while read -r nf; do
    # For each NF key, pull its nf and common-src commit/branch from the JSON
    nf_commit=$(jq -r --arg nf "$nf" '.[$nf].nf' "$NF_JSON_FILE")
    common_src_commit=$(jq -r --arg nf "$nf" '.[$nf]["common-src"]' "$NF_JSON_FILE")

    # If common-src commit is empty, log warning
    if [ -z "$common_src_commit" ]; then
        log "WARNING" "No common-src commit provided for $nf"
    fi

    NF_MAP["$nf"]="$nf_commit"
    COMMON_SRC_MAP["$nf"]="$common_src_commit"

done < <(jq -r 'keys[]' "$NF_JSON_FILE")

# --- Build function ---
build_nf() {
    NF_NAME=$1
    NF_COMMIT=$2
    COMMON_SRC_COMMIT=$3
    CPU_START=$4
    CPU_END=$5

    NF_DIR="$CLONE_DIR/oai-cn5g-${NF_NAME}"
    LOG_FILE="$LOG_ARCHIVES/${NF_NAME}.log"

    {
        log "INFO" "Processing NF: $NF_NAME at commit/branch: $NF_COMMIT (CPUs ${CPU_START}-${CPU_END})"
        rm -rf "$NF_DIR"

        # Clone repo (fresh every build for a hermetic checkout).
        if ! git clone "${BASE_URL}-${NF_NAME}.git" "$NF_DIR"; then
            log "ERROR" "Clone failed for $NF_NAME"
            echo "$NF_NAME" >> "$FULL_FAILED_NFS_PATH"
            exit 1
        fi

        cd "$NF_DIR"

        # Checkout NF commit/branch
        if ! git checkout "$NF_COMMIT"; then
            log "ERROR" "Failed to checkout $NF_COMMIT for $NF_NAME"
            echo "$NF_NAME" >> "$FULL_FAILED_NFS_PATH"
            exit 1
        fi

        if ! git submodule update --init --recursive; then
            log "ERROR" "Submodule update failed for $NF_NAME"
            echo "$NF_NAME" >> "$FULL_FAILED_NFS_PATH"
            exit 1
        fi

        # Checkout common-src
        COMMON_SRC_PATH=$(git config --file .gitmodules --get-regexp path | grep common-src | awk '{print $2}')
        if [ -d "$COMMON_SRC_PATH" ]; then
            if [ -n "$COMMON_SRC_COMMIT" ]; then
                # The submodule clone only has its pinned commit; fetch the
                # requested common-src ref (PR branch, tag, or commit) first.
                if git -C "$COMMON_SRC_PATH" fetch origin "$COMMON_SRC_COMMIT"; then
                    COMMON_SRC_REF=FETCH_HEAD
                else
                    git -C "$COMMON_SRC_PATH" fetch origin || true
                    COMMON_SRC_REF="$COMMON_SRC_COMMIT"
                fi
                if ! git -C "$COMMON_SRC_PATH" checkout "$COMMON_SRC_REF"; then
                    log "ERROR" "common-src checkout failed for $NF_NAME"
                    echo "$NF_NAME" >> "$FULL_FAILED_NFS_PATH"
                    exit 1
                fi
            else
                log "ERROR" "common-src checkout failed for $NF_NAME because commit is empty"
                echo "$NF_NAME" >> "$FULL_FAILED_NFS_PATH"
                exit 1
            fi
        else
            log "WARNING" "No common-src submodule found for $NF_NAME"
        fi

        NF_SHA=$(git rev-parse --short=8 HEAD)
        COMMON_SRC_SHA=$(git -C "$COMMON_SRC_PATH" rev-parse --short=8 HEAD)
        log "INFO" "NF: $NF_NAME | NF commit: $NF_SHA | common-src commit: $COMMON_SRC_SHA"

        # Submodule status
        git submodule status | while read line; do
            log "INFO" "Submodule: $line"
        done

        # Build Docker image, tagged for the private registry when PRIVATE_REGISTRY is set.
        IMAGE_TAG="${NF_NAME}-${NF_SHA}-common-src-${COMMON_SRC_SHA}"
        if [ -n "$REGISTRY" ]; then
            IMAGE_REF="${REGISTRY}/oai-${NF_NAME}:${IMAGE_TAG}"
        else
            IMAGE_REF="oai-${NF_NAME}:${IMAGE_TAG}"
        fi
        log "INFO" "NF Docker Image: $IMAGE_REF"
        if ! DOCKER_BUILDKIT=0 docker buildx build \
            --no-cache \
            --target "oai-${NF_NAME}" \
            --tag "$IMAGE_REF" \
            --cpuset-cpus="${CPU_START}-${CPU_END}" \
            --build-arg GIT_COMMIT="${NF_SHA}" \
            --file "docker/Dockerfile.${NF_NAME}.ubuntu" .; then
            log "ERROR" "Docker build failed for $NF_NAME"
            echo "$NF_NAME" >> "$FULL_FAILED_NFS_PATH"
            exit 1
        fi

        # Log the human-readable size (the report parses the 'Image Size:' line).
        IMAGE_SIZE=$(docker images --format '{{.Size}}' "$IMAGE_REF" | head -n1)
        log "INFO" "Image Size: ${IMAGE_SIZE}"

        # Push to the private registry (only when PRIVATE_REGISTRY is set).
        if [ -n "$REGISTRY" ]; then
            log "INFO" "Pushing $IMAGE_REF"
            if ! docker push "$IMAGE_REF"; then
                log "ERROR" "Docker push failed for $NF_NAME"
                echo "$NF_NAME" >> "$FULL_FAILED_NFS_PATH"
                exit 1
            fi
            docker rmi "$IMAGE_REF" || true
        fi
        cd - &> /dev/null
        log "INFO" "Finished building $NF_NAME (see $LOG_FILE)"
    } > "$LOG_FILE" 2>&1

}

# --- Helper function to wait for a batch of PIDs ---
wait_for_batch() {
    local batch_pids=("$@")
    local batch_failed=0

    for pid in "${batch_pids[@]}"; do
        if ! wait "$pid"; then
            batch_failed=1
        fi
    done

    if [ "$batch_failed" -ne 0 ]; then
        log "WARNING" "One or more NF builds failed in this batch; continuing with remaining NFs"
    fi
}

# --- Launch NF builds in parallel ---
for nf in "${!NF_MAP[@]}"; do
    # Assign CPU cores in the parent so the NUMA counters persist across NFs.
    if (( NUMA1_FIRST_IDX < 8 )); then
        CPU_START=$(( NUMA1_FIRST_HALF_START + NUMA1_FIRST_IDX * CORES_PER_BUILD ))
        NUMA1_FIRST_IDX=$(( NUMA1_FIRST_IDX + 1 ))
    else
        CPU_START=$(( NUMA1_SECOND_HALF_START + NUMA1_SECOND_IDX * CORES_PER_BUILD ))
        NUMA1_SECOND_IDX=$(( NUMA1_SECOND_IDX + 1 ))
    fi
    CPU_END=$(( CPU_START + CORES_PER_BUILD - 1 ))

    build_nf "$nf" "${NF_MAP[$nf]}" "${COMMON_SRC_MAP[$nf]}" "$CPU_START" "$CPU_END" &
    BATCH_PIDS+=($!) # store PID for batch tracking
    jobs_running=$(( jobs_running + 1 ))

    if (( jobs_running >= MAX_PARALLEL )); then
       log "INFO" "Batch PIDs: ${BATCH_PIDS[*]}"
        wait_for_batch "${BATCH_PIDS[@]}"
        # Reset for next batch
        BATCH_PIDS=()
        jobs_running=0
    fi
done

# --- Wait for remaining jobs in the final batch ---
if [ "${#BATCH_PIDS[@]}" -ne 0 ]; then
    wait_for_batch "${BATCH_PIDS[@]}"
fi

# --- Check if any NF builds failed ---
if [ -s "$FULL_FAILED_NFS_PATH" ]; then
    log "ERROR" "Some NF Docker builds failed. See $FULL_FAILED_NFS_PATH"
    exit 1
fi

log "INFO" "All NFs processed and Docker images built"
log "INFO" "Script finished at $(date)"
