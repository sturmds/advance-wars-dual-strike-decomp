# 0057: `fn.py diff` scores below 100% for hand-written asm whose ROM bytes match

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

For SDK and runtime functions written as inline asm, `fn.py diff` shows less than 100% although `make release` passes. Agents have to read the diff and decide that the difference is noise. Cases from 2026-10-11:

- A pool word shown as a symbol reference (b23-isd0, `Isd_RestoreFiq`, `Isd_RestoreIrq`). The original loads the pool word `Isd_RestoreFiq + 8` and branches to it; the C writes `ldr r1, =Isd_RestoreFiq + 8`. The diff shows 99.9%.
- A `dcd` word for an instruction the assembler lacks (b23-isd0, `Isd_Breakpoint`, `udf`, written as `dcd 0xe7ffffff`). objdiff reads the word as data and the score is 70%.
- `dcd` words for branches into another function (b26-rtf1: `_ffix` 95.4%, `_ffltu` 94%; b26-rtf0: `_fadd`, `_fsub` 99.8%). mwcc's assembler has no `b symbol+offset` and cannot branch to a label in another function.
- A relocation on a tail jump that the original does not have (b26-crt0, `do_autoload`, 99.77%): the final `b AutoloadCallback` is a relocation in our object and a plain branch in the original.
- `bl` instructions with no `relocs.txt` entry (b26-rtf1, `_dfix` and `func_02128438`, for example at 0x02128484): the original links the calls with no entry in `relocs.txt`, our object has none either, and `fn.py diff` marks the lines.

## Notes

- The ROM check decides here, so these differences are noise, not wrong code. The cost is reading time, and a real 99.8% cannot be told apart from a harmless one.
- b26/rti0 and b26/rtd1 found a way to avoid most `dcd` branches: write one asm function per entry point, in source order, each running into the next (same bytes), and list each as a function in `symbols.txt`. Branches then name their targets, and `-nodead` keeps the unreferenced ones. `ll_udiv.c` and `fp_compare.c` (46 asm functions) are the examples. It adds symbols that dsd would not have made.
- Related: 0017 (closed: table accesses that match), 0038 (`fn.py check` and `fn.py diff` disagree).

## Done when

- `fn.py diff` scores 100% for these cases, or prints a line such as "N lines differ only by relocation or `dcd`; ROM bytes equal" so an agent can tell.
- The one-asm-function-per-entry-point technique is in `docs/workflow.md`.
