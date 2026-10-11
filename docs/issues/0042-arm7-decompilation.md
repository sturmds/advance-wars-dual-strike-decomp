# 0042: ARM7 has no C at all

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** later

## Problem

All decompiled code so far is ARM9 (main, itcm and the overlays). The ARM7 binary (157,268 bytes at 0x02380000) is copied from `extract/` unchanged, and none of it is in C. The hard rules in `CLAUDE.md` keep it that way "until a later milestone". No issue tracks what that milestone needs.

## Notes

- Issue 0013 covers the build side: linking `arm7.bin` from delinked objects instead of copying it. That has to come first. This issue covers the step after it: replacing those objects with C.
- 0013 notes the string "2.39.00" and that ARM7 looks like the standard ARM7 component shipped to every DS game (the second CPU, which runs sound, touch screen, wireless and power). If it is unmodified library code, decompiling it is library work, closer to the library code in main and itcm than to game code.

Scope to decide:

1. Is any of ARM7 game code, or is it all stock SDK? Compare against other DS games that ship SDK 2.39 ARM7 components, and look for references to game data.
2. Which compiler and flags built it. It may not be mwccarm 2.0-72 with the ARM9 flags; ARM7 code targets the ARM7TDMI and is often Thumb.
3. Tooling. Whether dsd can delink and check ARM7 the way it does ARM9, or what we would add (0013 says `init` and `lcf` appear to handle ARM9 only).
4. Priority against the remaining ARM9 work: the partials and the ARM9 SDK/library code (742 functions).

## Plan

Order matters: each step needs the one before it.

1. **Link ARM7 from objects (issue 0013, done).** Find out whether dsd can delink a module at 0x02380000 with no overlays. If it can, write `config/AWRE/arm7/` (symbols, delinks, relocations) the way `tools/dsd_init.py` did for ARM9. If it cannot, record what is missing in `docs/tool-issues.md` and write the smallest tool of our own that splits `arm7.bin` into objects. Then link it with mwldarm, put the ARM7 hash in `config/AWRE/modules.sha1`, and keep `make release` matching. Close 0013 here.
2. **Answer scope questions 1 and 2.** Count ARM7's functions and how many are Thumb. Look for game data references and for code that differs from the SDK 2.39 ARM7 component. Fingerprint the compiler the way `docs/awre-facts.md` did for ARM9 (prologues, literal pools, `-proc arm7tdmi`), and record the result there.
3. **Teach the tools a second CPU (done).** `fn.py`, `fnindex.py`, `progress.py`, `propagate.py`, `trivial.py`, `permute.py` and the loop job templates assume ARM9 modules. Add `arm7` as a module with its own compiler flags in `tools/configure.py`. Put the C in `src/arm7/`, with `sdk/` and `game/` inside it if any game code turns up.
4. **Progress tracking** (see the requirement below), in the same change as step 3 (done).
5. **Decompile.** Start with ARM7 functions that have an ARM9 twin already matched (OS, PXI, and other SDK code built for both CPUs) through `propagate.py --module arm7`, then the short functions through `trivial.py --arm7`, then the rest through the loop (`fn.py pick --arm7`).
6. **Update the rules (done).** After step 1, change the hard rule in `CLAUDE.md` and the plan in `docs/early-phase-project-plan.md` so they no longer say ARM7 is copied from the ROM.

Priority: after the ARM9 game partials. Steps 1 to 4 can run beside the ARM9 SDK/library work, because they touch tools and config, not ARM9 C.

## Results

- **Step 1 (2026-10-11).** Done; issue 0013 is closed. dsd cannot delink ARM7 itself, so `tools/arm7.py` runs it on a stand-in ROM and links at the real addresses (`docs/tool-issues.md`). `config/AWRE/arm7/` holds one module, `arm7`, with its three parts (crt0 at 0x02380000, main RAM at 0x027c0000, WRAM at 0x037f8000) and names `func_arm7_XXXXXXXX` / `data_arm7_XXXXXXXX`.
- **Step 2, question 2 (2026-10-11).** 1,014 functions: 989 ARM, 25 Thumb (all BIOS call stubs). Compiler mwccarm 2.0 with `-O4,s -proc arm7tdmi -interworking`: the ARM9 compiler, optimised for size. Details in `docs/awre-facts.md`, "ARM7".
- **Step 2, question 1.** No game code found: no pointer into ARM9 memory and only system library strings. ARM7 counts as library code. Not yet compared with the ARM7 of another game built on library version 2.x.

- **Steps 3 and 4 (2026-10-11).** Done. `fn.py`, `fnindex.py`, `ds.py`, `propagate.py`, `trivial.py`, `permute.py`, `pragmas.py` and `triage_partials.py` handle the `arm7` module; how to use them is in `docs/workflow.md`, "ARM7". ARM7 stays out of `fn.py pick`, `propagate.py` and `trivial.py` unless asked for (`--arm7`, `--module arm7`), so the ARM9 loop runs as before. `PROGRESS.md` has ARM9 and ARM7 rows; its `game`, `SDK/library` and `all` rows add both CPUs.
- **Proof that the loop can use it.** In a scratch tree, `func_arm7_038076e8` (0x54 bytes; it looks like a PXI send that retries until the FIFO takes the word, then unlocks a mutex) went through `fn.py add`, `fn.py check` (match), `fn.py diff` (100%) and `fn.py finish`; `make release` passed with it linked from C, and `progress.py` counted it (ARM7 1 / 1014). It was not committed. That run found that the functions `arm7_init.py` adds after dsd had no call relocations; the init script now adds them.
- **Step 5, first numbers.** Dry runs on mainline after these changes: `propagate.py --module arm7` finds 52 ARM7 functions that compile to the original from the C of matched functions (47 identical twins, 5 near twins, from ARM9 templates), and `trivial.py --arm7` 114 short ones. That is about 166 of 1,014 functions with no model tokens.
- Open: 25 pointer-like ARM7 words without relocations, mostly section ends (issue 0054). 
- **Step 6 (2026-10-11).** Done: the hard rule, pipeline, compiler and source layout lines in `CLAUDE.md`, and `docs/early-phase-project-plan.md`, say ARM7 is linked from objects.

## Requirement: PROGRESS.md tracks ARM7

- ARM7 gets its own rows, separate from ARM9, in both the totals table and the by-module table. ARM9 means main, itcm and the overlays; ARM7 means `arm7.bin`.
- Each CPU keeps the game and SDK/library split, if ARM7 turns out to have any game code.
- The totals table ends with a combined row that adds ARM9 and ARM7: functions in C, bytes in C, % of bytes, and asm functions and bytes.
- Once ARM7 is delinked (step 1), ARM7's functions count in the denominators even while none of them are in C, so the combined percentage reflects all the code there is to decompile.
- `tools/progress.py` and `build/report.json` follow the same split.

## Other code in the ROM

The Download Play child in the file system has its own ARM9 and ARM7. It is tracked in issue 0049, not here.

## Done when

Steps 1 and 2 are done and their answers are written here, and the plan above is updated with what they found. Split step 5 into its own issue then, and close this one.
