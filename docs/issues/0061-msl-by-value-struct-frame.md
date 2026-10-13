# 0061: MSL functions that mwccarm 2.0-72 cannot match because of the by-value struct frame

- **Status:** closed (2026-10-12)
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

### Evidence found (2026-10-12)

- **No library object in metroskrew holds these functions.** The ROM bytes of the nine functions (relocations masked) were searched for in every `.a` under `.cache/tools/metroskrew/lib/` (DS 1.2 base to sp4, DS 2.0, DSi 1.1 to 1.6sp1, all variants). None was found. `long2str` and `longlong2str` were found, in `MSL_C_NITRO_Ai_LE.a` of DS 1.2 base, base_b73, sp2, sp3 and sp4: the ROM's MSL comes from that archive's line, but not from any of these builds.
- **The 1.2 objects have the frame the ROM lacks.** In DS 1.2 base `MSL_C_NITRO_Ai_LE.a`, `printf.o` has `__pformatter` at 0x880 bytes and `double2hex` at 0x3c0 (ROM: 0x83c and 0x3b4), and `float_exception` has `callHandler` at 0x10c (ROM `__fp_trap`: 0x100). Its `__fp_handler_32_to_32` has the same `r11` frame and `sp` moves that 2.0-72 builds from our C. No archive has the ROM's sizes. The game was linked with an MSL built by a compiler that metroskrew does not include.
- **No compiler build matches.** Each file was compiled with every metroskrew mwccarm (2.0-72 to 87, 3.0-114 to 139, 4.0-1018 to 1051), at `-O4,p`, `-O4,s`, `-O3,p`, `-O3,s`, `-O2`, `-O1` and `-O0`, for `arm946e`, `arm7tdmi` and `arm9tdmi`. No combination matches any of the nine. Fewest differing rows, against 2.0-72 `-O4,p` in brackets: the trap wrappers 12-16 with 3.0 or 4.0 (17-22), `__fp_trap` 22 with 3.0 (30), `double2hex` 121 and `__pformatter` 284, both best with 2.0.
- **What the original's code looks like.** It keeps 2.0's ARMv4 prologue and epilogue (`stmdb sp!, {lr}` ... `ldmia sp!, {lr}; bx lr`) but builds the struct argument copy as 3.0 does: `sub r2, sp, #4` into an outgoing area reserved in the fixed frame, with no `r11` and no `sp` change. 3.0 builds the copy the same way but saves `{r3, lr}` and orders the loads differently. The original compiler looks like a build other than those in metroskrew.
- **mwccarm 2.0 build 56 matches.** It is in the CodeWarrior for DS compiler set linked from the DS decomp Discord (`https://decomp.aetias.com/files/mwccarm.zip`, `mwccarm/1.2/b56/`), and not in metroskrew. With the ARM9 flags it builds all eight trap and printf functions byte for byte from the C as it was. `double2hex` went from 121 to 52 differing rows, and matched after three changes to the C: a separate pointer for the byte swap, the exponent in two steps, and the declaration order.
- **Only for these files.** Build 56 makes 12 other MSL functions that match with 2.0-72 differ by 3 to 21 rows. The three game partials score the same with both builds.
- **wine, not wibo.** wibo 1.2.0 leaves the x87 FPU at Linux precision, and mwccarm then lowers `__pformatter`'s switch differently (`docs/tool-issues.md`).

## Resolution

sturmds chose option 1, with build 56 under wine. `tools.lock` pins the zip (`[mwccarm-b56]`; `setup.py` unpacks only `mwccarm/1.2/b56/`). `config/AWRE/compilers.txt` lists the nine files, `configure.compiler_for()` reads it, and `tools/wine_cc.py` runs the compiler under wine with a prefix in `.cache/wine`. All nine functions are linked from C. CLAUDE.md's Linux-only rule names this one exception.

## Done when

- sturmds picks one of the three options, and the functions above are linked from C or marked as left on purpose.
