# 0071: Remaining work to list the project on decomp.dev

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-12
- **Milestone:** later

## Problem

decomp.dev reads only GitHub Actions artifacts from public repositories (`docs/third-party/decomp-dev.md`), and CI cannot build: the build needs `orig/AWRE.nds`, which never leaves the builder's machine.

## Done

- sturmds lifted the "no decomp.dev" rule (2026-10-13) and made https://github.com/sturmds/advance-wars-dual-strike-decomp.
- `hooks/pre-commit` and `land_self.py` commit the report `make release` writes as `PROGRESS.json`, next to `PROGRESS.md`. `apply_commit.py`, `rebase_onto_mainline.py` and `review_stamp.py` treat it as they treat `PROGRESS.md`. `.gitattributes` marks it `-diff`: it is one 6.3 MB line.
- `.github/workflows/decomp-dev.yml` uploads `PROGRESS.json` as `report.json` in artifact `0.1_report` on every push to `mainline`.
- asm functions: `config/AWRE/asm_functions.txt` decides which count (commit 602767fcf).
- The total: `PROGRESS.md` reads the report, so both count code in dsd gap units and agree.

## What is left

1. **Make `mainline` the default branch on GitHub.** decomp.dev reads only completed `push` runs on the default branch.
2. **Register the project.** Log in to decomp.dev with GitHub, open `/manage/new`, pick the repository and the platform "Nintendo DS" (`nds`). Without decomp.dev's GitHub App the site looks for new runs every 30 minutes; with it, a webhook tells it.
3. **Version label.** The artifact is `0.1_report`, so decomp.dev shows the version "0.1". Other projects use the game code (`YKGP_report` in khdays-decomp); ours would be `AWRE_report`. Change `VERSION` in `tools/decomp_dev_report.py` and the artifact name in the workflow together.
4. **Optional: progress categories.** Units can carry `progress_categories` (main, each overlay, SDK, ARM7), and decomp.dev then shows a bar for each.

## Notes

- decomp.dev's list shows code only. Data is barely built from C (`complete_data` is near 0%).
- objdiff scores 21 asm functions in linked files (6,976 bytes, such as `_dmul` and `Isd_DisableFiq`) below 100% although the ROM matches; see issue 0057.
- The artifact name rule and the two measures shown come from decomp.dev's source, commit `e9c086adb`.
- The workflow uses `actions/checkout@v4` and `actions/upload-artifact@v4` by tag, not by commit SHA.

## Done when

The project shows on https://decomp.dev/?platform=nds with the same linked percentage `make release` prints.
