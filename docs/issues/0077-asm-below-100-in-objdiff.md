# 0077: objdiff scores some listed `asm` functions below 100% although the ROM matches

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

`tools/decomp_dev_report.py` counts the functions in `config/AWRE/asm_functions.txt` as decompiled, but only where objdiff scores them 100%. On 2026-10-13, 21 functions in `complete` units score lower, although `make check` passes, so their bytes match. All are `asm`; most are on the list, so they count as linked but not as decompiled. Examples:

- Isd_DisableFiq, Isd_DisableIrq (97.5%): the original's branch into the next function shows as `b 0x10`, ours as `b Isd_RestoreFiq-0x8`.
- _dadd, _dsub (98.75%): a branch into a neighbour shows as `b` on one side and `bmi` on the other.
- Isd_Breakpoint (70%), _fdiv (91%), _ffix, _ffltu, func_02128438, _dfix, _dmul, _fadd, _fsub, FCmp_NanAFlags, DCmp_NanA, DCmp_NanAFlags, _ll_sdiv_common and its ARM7 copy, Bios_AwaitIrqFlags_Arm7, Isd_RestoreFiq, Isd_RestoreIrq.

The rows that differ are branches to or from a neighbouring function and data words in code, which objdiff decodes differently on the two sides.

## What would fix it

A rule like issue 0074's `pool_word` for each row kind, in `fn.py` and so in the report: a branch whose target resolves to the same address on both sides. Check each against the ROM so that no real difference is hidden.
