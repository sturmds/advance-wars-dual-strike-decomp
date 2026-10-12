# 0068: permuter.py reports "0 outputs" on files with `asm` declarations, without running

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-12
- **Milestone:** M2

## Problem

decomp-permuter parses `build/permuter/NAME/base.c` with its copy of pycparser, which rejects mwcc's `asm void _ll_udiv_common(void);`. `include/sdk/runtime.h` has six such declarations (since 8511870fb and d4fe907d8, 2026-10-11), and `include/sdk/os_arm7.h` includes it (since 5333792cd, 2026-10-12). decomp-permuter then prints the syntax error and exits 1, but `tools/permuter.py` sent that output to `permuter.log` and did not check it: the summary said "0 outputs", which reads like a full run that found nothing. `--explain` did not parse base.c at all, so it worked on these files and gave no sign.

Reach: every C file that includes `runtime.h`, directly or through `os_arm7.h` (and so `os.h`, `wl.h`, `wmsp.h`, `rtc.h`, `spi_thread.h`, `card_arm7.h`). That covers 244 files under `src/arm7/` and four ARM9 files in `src/sdk/runtime/` (the divide helpers). A permuter run on ARM7 code after 2026-10-12 06:58 did not run. Found by b34/arm7 on the WM files.

Of the 46 functions in PROGRESS.md "Partial functions" on 2026-10-12, two failed before the fix and work after it:

- func_arm7_027c6020 (`src/arm7/sdk/wm/func_arm7_027c6020.c`)
- func_arm7_027c632c (`src/arm7/sdk/wm/func_arm7_027c632c.c`)

The other 44 parsed before and after. With the fix, for all 46, the base as decomp-permuter prints it compiles with the build's mwccarm flags.

## Fixed (2026-10-12)

`tools/permuter.py` writes the preprocessed file twice: `pre.c` as mwcc gives it, and `base.c` for decomp-permuter, where each line matching `MWCC_ONLY` (an `asm` declaration) becomes `#pragma _permuter b64literal <the line in base64>`. The parser accepts the pragma, and decomp-permuter writes the original line back when it prints the base or a candidate, so mwcc compiles the real text. Lines stay one for one, so line numbers in errors hold. `--explain` and the rescoring of the unchanged function compile `pre.c`.

`setup` parses `base.c` with decomp-permuter's parser and stops with its message if that fails, naming `MWCC_ONLY` as the place to add a new construct. A run that ends without a `base score` line in `permuter.log` stops with the log's last 20 lines instead of printing a summary.

Test: `tools/tests/test_permuter.py` (in `make test`) checks that `parser_safe` hides two `asm` declarations, keeps the line count, that the parser rejects the raw text and accepts the cleaned one, and that the printed source has the declarations back.
