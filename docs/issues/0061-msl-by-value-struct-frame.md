# 0061: MSL functions that mwccarm 2.0-72 cannot match because of the by-value struct frame

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

Some C library (MSL) functions pass a struct or double by value. mwccarm 2.0 (every build from 72 to 87) moves `sp` around the call and sets up an `r11` frame pointer. The original has neither, so the frame and the stack offsets differ and these stay below 100%:

| Function | Score | What differs |
|---|---|---|
| Six soft-float trap wrappers (`func_02121d58`, `func_02121db0`, `func_02121e0c`, `func_02121e58`, `func_02121ea8`, `func_02121ef4`; since renamed `__fp_trap_*`) and the dispatcher `__fp_trap` (`func_02121f3c`), b23-msl0 | 52-85% | `r11` frame plus `sp` adjust for the split-struct argument copy; the case tree and body match |
| `double2hex`, b23-msl3a | 86.82% | `expFormat` passed by value to `long2str`; 3.0 builds compile it the original's way |
| `__pformatter`, b23-msl2 | 34% | `PrintFormat` passed by value through a 12-byte outgoing argument area below `sp`; 3.0 drops the frame pointer but picks other registers |

`docs/awre-facts.md` says the linked MSL and runtime code equals the CodeWarrior 1.2 library objects that metroskrew ships (42 functions, 11.6 KB). These functions were probably built by an older compiler than the 2.0-72 used for the rest.

## Notes

Options for sturmds to decide:

1. **A per-file compiler.** `tools/configure.py` takes the compiler version per source file (a table in `config/AWRE/`), and these files build with another mwccarm that metroskrew ships (1.2 base to sp2, or 3.0). Costs: another compiler path in the build and a version note per file. It fits the evidence that the MSL objects came from the 1.2 library.
2. **Write the files as asm**, as `fp_compare.c` and `ll_udiv.c` do. It always matches, but gives no readable C, and the build stays as it is.
3. **Leave them as they are**: the partial C stays in `src/` unlinked and the original bytes link from the ROM.

Evidence needed before choosing:

- Whether any compiler build in the metroskrew set both builds the by-value frame the original's way and picks the same registers. Known: 3.0 builds fix the frame for `double2hex`; for `__pformatter` 3.0 drops the frame pointer but allocates differently. Not tried: the 1.2 base through sp2 builds on these functions (the 1.2 evidence came from comparing library objects, not from compiling our C).
- Whether the 1.2 library objects equal the ROM bytes for these functions (the count of 42 does not say which functions).
- Which flags such a per-file compiler needs (the SDK flags may not apply).

`docs/awre-facts.md` gets a pointer to this issue, nothing more.

## Done when

- sturmds picks one of the three options, and the functions above are linked from C or marked as left on purpose.
