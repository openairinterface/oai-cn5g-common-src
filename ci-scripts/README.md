<!-- SPDX-License-Identifier: CC-BY-4.0 -->

# CI Pipeline Documentation for `OAI-CN5G-COMMON-SRC`

## 1. Overview

The CI pipeline determines:

* Which branch or commit of each NF to use
* Which `common-src` branch or commit to associate with each NF

The pipeline is triggered in the following cases:

* A Pull Request targeting the `develop` branch.
* A Push to the `develop` branch.

The behavior differs depending on whether the trigger is a **Pull Request (PR)** or a **Push event**. A PR is detected from the `GITHUB_PR_NUMBER` environment variable; when it is absent the run is treated as a push.

The pipeline runs these stages in order:

1. **Initialize** – validate job parameters and compute the enabled NFs.
2. **Prepare Source Metadata** – detect PR vs push and check out the source.
3. **Verify Labels** *(PR)* – skip builds for documentation-only changes.
4. **Verify Guidelines** *(PR)* – run the contribution checks.
5. **Prepare NF Configuration** *(PR)* – build `nf_map.json` from the PR description.
6. **Build Ubuntu Images** *(PR)* – build the per-NF Docker images.

## 2. Network Functions Covered by CI

The CI pipeline knows about the following eleven Network Functions:

```groovy
ALL_NFS = ['amf', 'ausf', 'lmf', 'nef', 'nrf', 'nssf', 'pcf', 'smf', 'udm', 'udr', 'upf']
```

This list is defined in the Jenkins script (`ci-scripts/Jenkinsfile-GitHub-Common-Src`).

Which of these are actually built in a given run is controlled by the per-NF `do<NF>` boolean job parameters (`doAMF`, `doAUSF`, `doLMF`, …). A missing parameter defaults to `true`, so every NF is included unless a job explicitly disables it. This means newly added NFs are built by default until a job opts them out. The resulting set is referred to as the **enabled NFs**.

## 3. Pull Request (PR) Workflow

### 3.1 JSON-Based Configuration

For Pull Requests, the CI pipeline reads a JSON block from the **PR description**. The PR body is fetched through the GitHub API, and the fenced ```` ```json ```` block is extracted from it.

This JSON block defines:

* The branch or commit of each Network Function
* The branch or commit of the `common-src` submodule

To make this convenient, the JSON scaffold is kept in the repository's PR template (`.github/pull_request_template.md`). The template pre-populates the PR body so contributors only edit the values. The template is optional — CI parses whatever JSON block is present in the PR description regardless of how it got there.

### 3.2 Example JSON Configuration

```json
{
  "AMF": ["develop", "develop"],
  "SMF": ["my-feature-branch", "b5042f5f"],
  "UDM": ["my-udm-branch"]
}
```

* `AMF` – NF branch + common-src branch
* `SMF` – NF branch + common-src commit SHA
* `UDM` – NF branch only → common-src falls back to the PR source branch

Each entry follows this format:

```json
"NF_NAME": ["nf_branch_or_commit", "common_src_branch_or_commit"]
```

* **First value** – Branch or commit of the Network Function repository
* **Second value** – Branch or commit of the `common-src` submodule

The block is validated before use. The build fails on invalid JSON, an unknown NF name, a duplicate entry, a malformed (non-array) value, or a missing NF ref.

### 3.3 Handling Missing Network Functions in a PR

The pipeline iterates over the enabled NFs and fills in anything not listed in the JSON block:

```groovy
enabledNFs.each { nf ->
  if (!nfNames.contains(nf)) {
    nfNames.add(nf)
    nfRefs.add(defaultNFBranch)   // 'develop' unless overridden by the defaultNFBranch param
  }
}
```

If a Network Function is not present in the JSON block:

* It is automatically added.
* Its NF branch defaults to `defaultNFBranch` (`develop` by default).
* Its `common-src` ref defaults to the PR source branch (`GITHUB_PR_SOURCE_BRANCH`).

Within a listed NF's array, the two values behave differently:

* The **first value (NF ref) is required.** If it is empty or missing, the build fails (`missing NF branch/commit ref`).
* The **second value (common-src ref) is optional.** If you provide only the NF ref (e.g. `"SMF": ["my-branch"]`) or leave the second value empty, CI uses the PR's source branch (`GITHUB_PR_SOURCE_BRANCH`) as the common-src ref for that NF.

### 3.4 Contribution Guideline Checks

On every PR, the `Verify Guidelines` stage runs `ci-scripts/pre-ci-check.sh` against the PR branch.

## 4. Push Event Workflow

Push events **do not build the Network Functions**. In the `Prepare Source Metadata` stage the pipeline sets `doBuild = false` for any non-PR trigger, and the two build stages (`Prepare NF Configuration` and `Build Ubuntu Images`) are guarded by `when { doBuild }`, so they are skipped.

The reason is what the CI is validating. On a Pull Request, `common-src` carries a *proposed* change that has not landed yet, so CI rebuilds each enabled NF against that proposed `common-src` to prove the change is safe **before** it merges. A push to `develop` is that same change *after* it has already been merged and validated — rebuilding it would only repeat work that the PR already did, so the NF build sweep is intentionally PR-only.

As a result, on a push the pipeline only initializes, prepares source metadata, and posts a GitHub commit status against the triggering commit (`GIT_COMMIT`). The label and guideline stages are gated on `isPullRequest` and are skipped as well.

## 5. Docker Build Behavior

### 5.1 Build Skipping via Labels (PR Only)

The label check runs only on Pull Requests. If a PR carries the label https://github.com/openairinterface/oai-cn5g-common-src/labels/documentation

then:

* The NF Docker builds are skipped (`doBuild = false`).
* The contribution guideline checks still run.

This lets documentation-only changes bypass the builds. No other labels affect build gating.

### 5.2 Parallel Batch Execution

The NF images are built by `ci-scripts/build_ubuntu_core_images.sh`:

* Docker builds for Network Functions run in parallel batches, with CPU cores pinned per NF build.
* Image builds run on Pull Requests only (see [Push Event Workflow](#4-push-event-workflow)); all Docker images are built with the `--no-cache` option.
* Currently, Docker images are built only for the AMD64 architecture.
* If any Network Function fails in a batch:

  * The script exits immediately.
  * Subsequent batches do not run.

This ensures early failure detection and efficient resource usage without overloading the build node.

### 5.3 Status Reporting

For Pull Requests, the pipeline reports back to GitHub:

* A per-stage commit status (GitHub check) for each stage.
* A PR comment summarising the run, including a table of what CI built (NF ref and `common-src` ref per NF) and any failing stages or guideline warnings.

For Push events, the commit status is posted against the triggering commit.
