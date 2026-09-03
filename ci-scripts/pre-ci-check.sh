#!/bin/bash
# SPDX-License-Identifier: MIT

function usage {
    echo "OAI GitHub PR validation script (CLA, Signed-off-by, commit signature and merge commits)"
    echo ""
    echo "Usage:"
    echo "------"
    echo "    $0 -s <source-branch> -t <target-branch> -r <owner/repo>"
    echo "        -p <pull-request-number> -c <cla-csv>"
    echo ""
    echo "Options:"
    echo "--------"
    echo "    -s"
    echo "    The source branch of the pull request. Default value is current Git Branch (HEAD)"
    echo ""
    echo "    -t"
    echo "    The target branch of the pull request. Default value is develop"
    echo ""
    echo "    -r"
    echo "    The repository in owner/repo form, used to verify commit signatures"
    echo "    through the GitHub API. Without it the signature check is skipped."
    echo ""
    echo "    -p"
    echo "    The pull request number, used to verify commit signatures through the"
    echo "    GitHub API. Defaults to \$GITHUB_PR_NUMBER when the pipeline exports"
    echo "    it. Without it the signature check is skipped."
    echo ""
    echo "    -c"
    echo "    CSV file listing the emails that signed the CLA. Addresses at a"
    echo "    domain covered by a corporate CLA pass without being listed, see"
    echo "    CLA_DOMAINS in this script. Without it the CLA check is skipped."
    echo ""
    echo "    -h"
    echo "    Print this help message."
    echo ""
}

# Parse arguments properly
SOURCE_BRANCH=$(git rev-parse --abbrev-ref HEAD)
TARGET_BRANCH="origin/develop"
GITHUB_REPO=""
CLA_CSV=""
# The pipeline exports GITHUB_PR_NUMBER, so -p is optional there.
PR_NUMBER="${GITHUB_PR_NUMBER:-}"

while getopts ":s:t:r:p:c:h" opt; do
    case "$opt" in
        s)
            SOURCE_BRANCH="$OPTARG"
            ;;
        t)
            TARGET_BRANCH="$OPTARG"
            ;;
        r)
            GITHUB_REPO="$OPTARG"
            ;;
        p)
            PR_NUMBER="$OPTARG"
            ;;
        c)
            CLA_CSV="$OPTARG"
            ;;
        h)
            usage
            exit 0
            ;;
        :)
            echo "Error: Option -$OPTARG requires a value."
            echo ""
            usage
            exit 2
            ;;
        \?)
            echo "Error: Invalid option -$OPTARG"
            echo ""
            usage
            exit 2
            ;;
    esac
done

if [[ "$SOURCE_BRANCH" =~ ^[0-9a-f]{40}$ ]]; then
  # note: if no branch could be found, it will result in "" and git rev-list
  # will use the commit ID. Exclude "HEAD detached at", then use first branch
  # name.
  BRANCH_NAME=$(git branch -a --points-at $SOURCE_BRANCH --format='%(refname:short)' | grep -v detached | head -n1)
  echo "SHA recognized in $SOURCE_BRANCH, using \"$BRANCH_NAME\" as branch name"
else
  BRANCH_NAME="$SOURCE_BRANCH"
fi

# ----------------------------
# Both ends of the commit range
# ----------------------------
# A ref that does not resolve makes git rev-list return nothing, and every
# check below would then pass on an empty range. A fetch that half-failed must
# stop the pipeline, not clear it.
for ref in "$TARGET_BRANCH" "$SOURCE_BRANCH"; do
    if ! git rev-parse --verify --quiet "$ref" > /dev/null; then
        echo "Error: '$ref' does not resolve to a commit, the checks cannot run."
        exit 2
    fi
done

# ----------------------------
# Merged commits
# ----------------------------
# Checked before the CLA: a branch that was merged instead of rebased drags in
# commits that belong to other people, and the CLA failures they cause go away
# with the rebase. "Please rebase" is the message that fixes both.
mergeCommits=$(git rev-list --merges --abbrev-commit "$TARGET_BRANCH".."$SOURCE_BRANCH")
    if [[ -n "$mergeCommits" ]]; then
        message="Error: Following merge commits are found in the source branch history. Please rebase your branch.\n\n"
        message+="$(echo "$mergeCommits" | paste -sd ',' | sed 's/,/, /g')\n"
        message+="For more details, refer to the CONTRIBUTING file at the root of this repository."
        echo -e "$message"
        exit 3
    fi

# ----------------------------
# Check emails against the CLA
# ----------------------------
# Every email of the people behind the commits -- author, committer,
# Signed-off-by and Co-authored-by -- has to appear somewhere in the CSV,
# whatever its column layout.
# The checks are: author, committer, Signed-off by and Co-authored-by emails
#
# Organisations that signed a corporate CLA covering their staff: any address
# at these domains passes without being listed in the CSV. Matched exactly, so
# "eurecom.fr" does not cover "sub.eurecom.fr".
CLA_DOMAINS="openairinterface.org eurecom.fr phine.tech"

if [ -f "$CLA_CSV" ] && [ -r "$CLA_CSV" ]; then
    claEmails=$(grep -oE '[^,;[:space:]<>]+@[^,;[:space:]<>]+' "$CLA_CSV" | tr '[:upper:]' '[:lower:]' | sort -u)
    claDomainRe=$(echo "$CLA_DOMAINS" | tr ' ' '\n' | grep -v '^$' | sed 's/\./\\./g' | paste -sd '|')
    # grep -vxF keeps the commit emails that are not a line of claEmails.
    # One Markdown table row per offending identity: the pipeline posts this
    # text as a PR comment, where it renders as a table.
    claMissing=$(
        {
            # --abbrev-commit, so $c is both the commit to read and the SHA to show.
            for c in $(git rev-list --no-merges --abbrev-commit "$TARGET_BRANCH".."$SOURCE_BRANCH"); do
                # grep, not %(trailers): git drops the whole trailer block as soon
                # as the last paragraph holds a line that is not a trailer.
                trailers=$(git log -1 --format='%B' "$c" | grep -iE '^(Signed-off-by|Co-authored-by):')
                {
                    git log -1 --format='%ae%n%ce' "$c"
                    echo "$trailers"
                } | grep -oE '[^ ,;<>]+@[^ ,;<>]+' | tr '[:upper:]' '[:lower:]' | sort -u \
                  | grep -vxF "$claEmails" \
                  | grep -vE "@(${claDomainRe})\$" \
                  | awk -v sha="$c" '{ print sha "\t`" $0 "`\tNo signed CLA found" }'
                # A trailer holding no address names nobody, so it cannot be
                # cleared against the CSV and is not waived either.
                echo "$trailers" | grep -v '@' | grep -v '^$' \
                  | awk -v sha="$c" '{
                        key = $0; sub(/:.*/, "", key)
                        val = $0; sub(/^[^:]*:[[:space:]]*/, "", val)
                        print sha "\t`" val "` (`" key "`)\tNo email address could be resolved"
                    }'
            done
        # One row per identity, listing the commits it appears on, so a branch
        # of 50 commits from one unsigned author is one row and not fifty.
        # Without -r the SHA stays bare; GitHub links it by itself in a comment.
        } | awk -F'\t' -v repo="$GITHUB_REPO" '
            function link(s, r) { return r == "" ? s : "[" s "](https://github.com/" r "/commit/" s ")" }
            {
                k = $2 SUBSEP $3
                if (!(k in count)) { order[++n] = k; ident[k] = $2; issue[k] = $3 }
                count[k]++
                if (count[k] <= 10) shas[k] = shas[k] (count[k] == 1 ? "" : ", ") link($1, repo)
            }
            END {
                for (i = 1; i <= n; i++) {
                    k = order[i]
                    more = count[k] > 10 ? sprintf(" ... (%d commits)", count[k]) : ""
                    printf "| %s | %s | %s%s |\n", ident[k], issue[k], shas[k], more
                }
            }'
    )
    if [ -n "$claMissing" ]; then
        message="**CLA check failed**\n\n"
        message+="The following commit identities did not sign the CLA\n\n"
        message+="| Identity | Issue | Commit(s) |\n"
        message+="|---|---|---|\n"
        message+="$claMissing\n\n"
        message+="Please ensure that all commit authors, committers, \`Signed-off-by\` and "
        message+="\`Co-authored-by\` identities are covered by an approved CLA.\n\n"
        message+="You can find and sign the applicable CLA here: "
        message+="[Contributor License Agreement (CLA)](https://github.com/openairinterface/oai-cn5g-fed/wiki/Contributor-License-Agreement-%28CLA%29)\n"
        message+="For more details, please refer to the **CONTRIBUTING** file "
        message+="at the root of this repository or contact \`oaicicd@openairinterface.org\`."
        echo -e "$message"
        exit 2
    fi
# A path that was given but cannot be read is a misconfiguration, not a reason
# to let the commits through unchecked.
elif [ -n "$CLA_CSV" ]; then
    echo -e "CLA file '$CLA_CSV' is not a readable file."
    exit 2
else
    echo -e "No CLA file given (-c), emails were not checked against the CLA.\n"
fi

# ----------------------------
# Check commits missing Signed-off-by
# ----------------------------
unsignedCommits=$(
    for c in $(git rev-list "$TARGET_BRANCH".."$SOURCE_BRANCH" --no-merges); do
        if ! git log -1 --format=%B "$c" | grep -q "Signed-off-by:"; then
            git log -1 --format='%h' "$c"
        fi
    done | paste -sd ',' | sed 's/,/, /g'
)

# ----------------------------
# Check commits missing a verified signature
# ----------------------------
# GitHub holds every contributor's public key, so it can tell a valid signature
# from an unverifiable one. Only the 'verified' boolean is read, so GPG, SSH and
# S/MIME signatures are all accepted -- whatever GitHub marks as verified passes.
# The endpoint is public, no token needed.
#
# Every case that cannot check sets signatureCheckSkipped: an empty
# unverifiedCommits would read as "all signed".

unverifiedCommits=""
signatureCheckSkipped=""

if [ -z "$PR_NUMBER" ]; then
    signatureCheckSkipped="No pull request number given (-p), commit signatures were not checked."
elif [ -z "$GITHUB_REPO" ]; then
    signatureCheckSkipped="No repository given (-r <owner>/<repo>), commit signatures were not checked."
else
    apiResponse=$(mktemp)
    httpCode=$(curl -sS -m 20 -o "$apiResponse" -w '%{http_code}' \
        -H "Accept: application/vnd.github+json" \
        "https://api.github.com/repos/${GITHUB_REPO}/pulls/${PR_NUMBER}/commits?per_page=100")
    if [ "$httpCode" != "200" ]; then
        signatureCheckSkipped="Could not reach the GitHub API (HTTP $httpCode), commit signatures were not checked."
    # jq reads an empty body as "no output, exit 0", which would pass as an
    # empty commit list, so the body is checked before it is parsed.
    elif [ ! -s "$apiResponse" ]; then
        signatureCheckSkipped="Empty answer from the GitHub API, commit signatures were not checked."
    # jq exits non-zero when missing, or when the answer is not a commit array.
    elif ! unverifiedCommits=$(jq -r '[.[] | select(.commit.verification.verified == false) | .sha[:7]] | join(", ")' "$apiResponse" 2>/dev/null); then
        signatureCheckSkipped="Unexpected answer from the GitHub API, commit signatures were not checked."
    fi
    rm -f "$apiResponse"
fi

# ----------------------------
# Report
# ----------------------------
# Signed checks - exit 1. CLA and usage errors - exit 2. Merge commits - exit 3
#
# Printed first, so it shows when the Signed-off-by check fails too.
[ -n "$signatureCheckSkipped" ] && echo -e "$signatureCheckSkipped\n"

message=""

if [ -n "$unsignedCommits" ]; then
    message+="The following commit(s) are missing a Signed-off-by:\n\n$unsignedCommits\n\n"
    message+="Please use 'git commit -s' or 'git commit --signoff' to sign your commits.\n\n"
fi

if [ -n "$unverifiedCommits" ]; then
    message+="The following commit(s) are missing a verified signature:\n\n$unverifiedCommits\n\n"
    message+="Please use 'git commit -S' to sign your commits, with either a GPG or an SSH\n"
    message+="key, and make sure the matching public key is added to your GitHub account.\n\n"
fi

if [ -n "$message" ]; then
    message+="For detailed instructions, refer to the CONTRIBUTING file at the root of this repository."
    echo -e "$message"
    exit 1
fi

if [ -n "$signatureCheckSkipped" ]; then
    echo "All commits are signed off using 'git commit -s'."
else
    echo "All commits are signed off using 'git commit -s' and carry a verified signature."
fi
exit 0
