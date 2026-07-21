<!--
CI runs automatically on the PR:
- No label        -> full NF builds + checks.
- "documentation" -> skips NF Docker builds (docs-only PRs); guideline checks still run.
-->

## Network Function Source Configuration

<!--
Specify the NF and common-src refs to build below.
Format:  "NF": ["<nf-ref>", "<common-src-ref>"]
  - Key: NF name -- example: AMF, SMF.
  - 1st value: NF branch/commit (required).
  - 2nd value: common-src branch/commit (defaults to the PR's branch if omitted).

Omitted NFs build at 'develop'. Invalid JSON, an unknown NF, a duplicate,
a non-array value, or a missing NF ref fails the build.
-->

```json
{
  "NRF": ["develop", "common_src_branch"],
  "UPF": ["develop", "common_src_branch"],
  "AUSF": ["develop", "common_src_branch"],
  "UDR": ["develop", "common_src_branch"],
  "AMF": ["nf_feature_branch", "common_src_branch"],
  "UDM": ["develop", "common_src_branch"],
  "SMF": ["develop", "common_src_branch"],
  "PCF": ["develop", "common_src_branch"],
  "NSSF": ["develop", "common_src_branch"],
  "LMF": ["develop", "common_src_branch"]
}
```
