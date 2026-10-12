# 0071: Remaining work to list the project on decomp.dev

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-12
- **Milestone:** later

## Problem

`make release` writes the report decomp.dev reads (`tools/decomp_dev_report.py`, `build/artifacts/0.1_report/report.json`), but nothing publishes it. decomp.dev reads only GitHub Actions artifacts from public repositories (`docs/third-party/decomp-dev.md`). This needs the steps below, and `CLAUDE.md` says "No decomp.me, no decomp.dev" and "Work stays local". Only sturmds can change that rule; the steps wait on that decision.

## What is left

1. **A public GitHub repository**, where sturmds has admin rights. The decomp.dev form refuses others ("Repository must be public. Admin permissions are required."). The history pushed must pass `make pnc-check` (tenets 8-10). Agents never add remotes or push; sturmds does this.
2. **A workflow on the default branch, triggered by `push`.** decomp.dev reads only completed `push` runs on the default branch. Our branch is `mainline`; the GitHub default branch must be the one that gets pushed.
3. **Get the report into the workflow without the ROM.** CI cannot build: the build needs `orig/AWRE.nds`, which never leaves the builder's machine. The workflow can only upload a report written locally. Options:
   - Commit the report, as `PROGRESS.md` is committed, and have the workflow upload it as artifact `0.1_report`. The JSON is about 5.8 MB per commit; `objdiff-cli report generate -f proto` or a gzip would cut that, and we have to check that decomp.dev reads either form. Ask whether a report generated from our build fits tenet 9 ("Git holds only what we write"): it holds function names and sizes, not game data.
   - Commit a smaller summary and build the report in CI from it, as khdays-decomp's `tools/gen_report.py` does.
4. **Register the project.** Log in to decomp.dev with GitHub, open `/manage/new`, pick the repository and the platform "Nintendo DS" (`nds`). Without decomp.dev's GitHub App the site looks for new runs every 30 minutes; with it, a webhook tells it.
5. **Optional: progress categories.** Units can carry `progress_categories` (main, each overlay, SDK, ARM7), and decomp.dev then shows a bar for each.

## Decisions for sturmds

- **Version label.** The artifact is `0.1_report`, so decomp.dev would show the version "0.1". Other projects use the game code (`YKGP_report` in khdays-decomp); ours would be `AWRE_report`. It is the `VERSION` constant in `tools/decomp_dev_report.py`.
- **asm functions.** The report counts functions written as `asm` in .c files as neither matched nor linked, as `PROGRESS.md` does: 245 functions, 25,702 bytes. Counting them, as objdiff does by default, would show about 99.2% linked instead of 97.8%.
- **The total.** The report's linked bytes equal `PROGRESS.md`'s "Bytes in C" (1,804,888 at a0f9a43f0), but the percentage is 97.82% against 97.91%. objdiff's `total_code` (1,845,018) is 1,652 bytes more than `progress.py`'s total, which counts only functions (1,843,366). 1,628 of those bytes are code in dsd gap units with no function symbol (8 units, such as `_dsd_gap@itcm_1`, 648 bytes); the other 24 are not traced yet. Either name that code as functions in `symbols.txt`, or accept the difference.
- **Data.** decomp.dev's list shows code only. The report has `complete_data` at 6%, because most data is not built from C.

## Notes

- objdiff scores 21 asm functions in linked files (6,976 bytes, such as `_dmul` and `Isd_DisableFiq`) below 100% although the ROM matches; see issue 0057. Since the report leaves asm functions out, this does not change its numbers.
- The artifact name rule and the two measures shown come from decomp.dev's source, commit `e9c086adb`.

## Done when

The project shows on https://decomp.dev/?platform=nds with the same linked percentage `make release` prints, or sturmds decides not to publish it and this issue closes with that decision.
