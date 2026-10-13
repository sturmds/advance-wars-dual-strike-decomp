# 0077: objdiff scores some listed `asm` functions below 100% although the ROM matches

- **Status:** closed
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

## Cause

objdiff compares the dsd-delinked original's object with ours, and the two differ where the ROM does not:

- A branch into a neighbouring function in the same original object has no relocation there, only a destination (`b 0x10`); ours has a relocation (`b Isd_RestoreFiq-0x8`). `_dfix` and `func_02128438` call out of their object with no relocation on the original's side either.
- dsd writes `b #0` over every branch it relocates, so the original's object shows `b` where the ROM and ours have `bmi` (`_dadd`, `_dsub`; issue 0058).
- The original's object has no mapping symbols, so objdiff decodes data in code as instructions there (`_fdiv`'s table, `_dmul`'s and `_ffix`'s words, `Isd_Breakpoint`'s `udf`), and our Thumb `Bios_AwaitIrqFlags_Arm7` shows its pool word as two `lsl`. `_ffix`, `_ffltu`, `_fadd` and `_fsub` write branches into the next function as `dcd` words, which neither object relocates.
- dsd gives no relocation to Isd_RestoreFiq's and Isd_RestoreIrq's word that points into the function itself.
- dsd sized `_dfix` (0x144) and `func_02128438` (0x5c) without their last pool words, which our symbols cover (as in issue 0060).

## Fixed (2026-10-13)

`fn.score_rows` marks three more row kinds `=`, used by `fn.py diff`, `fn.py check` and `tools/decomp_dev_report.py`:

- `branch_target`: both rows are the same branch and reach the same address, through a relocation (symbol, addend, pc offset) or through objdiff's destination placed at the function's address. A `b` or `bl` from dsd against our conditional form matches only when the ROM word at that address has our condition.
- `resolved_word`: a plain `.word N` against our word relocated to a symbol whose address plus addend is N.
- `same_bytes`: no relocation on either side and the same bytes in both objects at the same place in the function, however objdiff decodes them.

`tools/tests/test_diff_rows.py` covers each rule and its failures. The 21 functions now score 100%; the report went from 99.27% decompiled, 99.42% linked to 99.42% and 99.42%.

Left open: the hand-encoded `dcd` branches carry no relocation, so a shifted build (`make shift`) would break them; the `_dfix` and `func_02128438` sizes in `symbols.txt` could be fixed as Isd_VFormat's was.
