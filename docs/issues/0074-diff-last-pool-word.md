# 0074: `fn.py diff` counts a differing row for Isd_VFormat's last literal pool word

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

After Isd_VFormat matched (batch 39, landed 460d05d65), the ROM matched but `fn.py diff Isd_VFormat` still printed `1 differ`. The row was the function's last literal pool word, `.word gIsdZeroString` at offset 0x1360.

## Cause

Two things together:

- `symbols.txt` gave Isd_VFormat size 0x1360, while its pool runs to 0x1364 (the delinks.txt range already ended at 0x0211fce4). objdiff cut the original's symbol before the word, so the row had only our side. dsd sized the symbol too small, as in issue 0060.
- With the size fixed, the row was still counted. The original's delinked object has no `$a`/`$d` mapping symbols, so objdiff tells pool words from code by the loads that read them. It showed the last word as `andeq r0, r0, r0` with the same `R_ARM_ABS32` relocation to gIsdZeroString that our `.word` has.

## Fixed (2026-10-13)

- Isd_VFormat's size in `config/AWRE/arm9/symbols.txt` is 0x1364.
- `fn.py` (`pool_word`) counts a row as matching (`=`) when both sides are a 4-byte word at the same offset with an `R_ARM_ABS32` relocation to the same address, and only one side is shown as `.word`. `fn.py diff Isd_VFormat` now gives 0 differ.

Other functions may have the same symbol size error; issue 0060 tracks a tool to list them.
