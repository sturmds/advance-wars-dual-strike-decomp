# tools/bss.py: requirements

Step 2 of issue 0082 (GitHub #1): define every `.bss` variable in C, so that the `.bss` in dsd gap units (1,955,164 bytes, 62% of all data) links from source files marked `complete`. This file says what the tool must do; review it before the tool is written.

## Background

- `.bss` is RAM that starts out zeroed: by crt0 for main, by the overlay loader for an overlay (how the SDK works in general; not yet read in this ROM's code). mwccarm writes it as `NOBITS` sections, so it has no bytes in the ROM. The ROM records only its bounds: main's `bss_start`/`bss_end` in the module parameters, and each overlay's `bss_size` in the overlay table (`extract/arm9/arm9.yaml`, `extract/arm9_overlays/overlays.yaml`).
- What `make check` can catch: an error that moves an address the ROM stores. Those are the pointers to `.bss` in code and data (literal pools, with relocations) and the bounds above. It cannot catch a wrong split between two such addresses (`u8 x[8]` against two `u32`), equal-size variables in swapped order, or a small error in the last variables of main, whose end the linker script rounds up to 32. objdiff does not catch these either, since its target sizes come from the `symbols.txt` the tool writes. So a matching build pins the addresses that something points at; the sizes and types between them come from our declarations, or are guesses for placeholders.
- mwccarm 2.0 puts each data or `.bss` variable of a file in its own section and orders the sections by a heapsort on size, starting from the reverse of the definition order (`docs/workflow.md`, "Several data objects in one file"). This holds for globals, `static` variables, function statics and zero-initialised globals. The linker's `ALIGNALL(4)` aligns each one to 4.
- So the `.bss` of one original file is a run of ascending sizes in the ROM. Where the size goes down, a new original file starts. A run can still hold several original files when their sizes happen to keep ascending.
- One file can hold only variables that form one ascending run, unless the pilot finds a way around the sort (open question 2).

| Module | `.bss` in gap units | dsd symbols | Declared in our C | Unaligned symbols |
|---|---:|---:|---:|---:|
| main | 1,307,968 | 649 | 475 | 81 |
| ov004 | 608,288 | 8 | 8 | 0 |
| ov003 | 16,672 | 29 | 15 | |
| arm7 (`.bss`, `.sbss`) | about 20,000 | 124 | 103 | |
| ov000, ov002, ov005, ov006 | 1,600 | 150 | 27 | |

## Requirements

### R1. Read what we know

- Modules: main, every ARM9 overlay with `.bss`, arm7 (`.bss` and `.sbss`). Take the `.bss` ranges from the module header in `delinks.txt` and the symbols in them from `symbols.txt`.
- Take each symbol's declared type from `build/dsindex.json` (`externs`) and its size from the struct sizes there and the basic types. Treat `extern T name[]` (no count) as unknown size.
- Take each symbol's users from `build/fnindex.json` (functions whose relocations point at it, and which `src/` file holds each function).

### R2. Build the variable list

- A variable starts at each dsd symbol that is 4-aligned and not inside another variable's declared size.
- A symbol that is unaligned, or inside a variable's declared size, is an alias: an address inside a variable that code uses. Keep its name. Define it in the linker script as `NAME = VARIABLE + OFFSET`, the way `maps.py lcf` does for `Ov000_gMapHeaders` (generalise that code; do not copy it).
- Size of a variable: the declared size, rounded up to 4, when known. Otherwise the gap to the next variable.
- Bytes that no symbol covers (after a variable's declared size, before the next symbol) are filler: variables that nothing in the code names. They get placeholder arrays named by address (`u8 data_0216e5c0[0x14]`, tenet 4). The tool may split one stretch of filler into several placeholders when that keeps a run ascending (R3).
- Alignment above 4: a variable whose address needs it (a gap before it that no size explains, and a 32-byte aligned address) gets the attribute mwcc needs, if mwcc has one (open question 3). Otherwise the tool stops and names the variable.

### R3. Group into as few files as possible

The one-function-per-file layout made too many files; `.bss` must not repeat that.

- Find the runs: split the variable list where the size goes down. Choose filler splits (R2) so as to make as few runs as possible. This is a small dynamic program over the variable list, since a filler stretch can be cut into any pieces that are multiples of 4.
- Host each run in an existing `src/` file when that is allowed (open question 1) and one file's functions use most of the run's variables. Otherwise make one new file per run.
- A new file goes in the directory where most of its variables' users are (`src/game/main/copowers/`, `src/game/main/map/`). It goes at the module level only when the users are spread out or unknown. Name it after what it holds when the users share a topic (`map_state_bss.c`), otherwise after its first variable's address (`bss_0216e020.c`).
- Within a file, define the variables in the order `definition_order` gives. Move `mwcc_heapsort` and `definition_order` from `tools/maps.py` into a shared module (for example `tools/mwcc_order.py`), and make `maps.py` and `tools/tests/test_data_order.py` use it.
- Print the run count before and after the filler choice, and the count of new files against hosted runs.

### R4. Write the C

- Each variable gets its declared type and name. The file includes the header that already declares it, so the compiler checks that the types agree. A variable declared only inside one `.c` file gets its declaration moved to a header the file can include, or is defined with the same type and a comment naming the other file; prefer the header.
- A variable used only inside its new file becomes `static` only when no other file names it. Do not rename anything.
- Format the files (`tools/format.py`) and follow `TENETS.md`. Comments only for what the code cannot say (why a placeholder has its size).

### R5. Write the config

- `delinks.txt`: add a `.bss start:… end:…` range to each new or hosting file's entry, and mark new files `complete`. Keep the file's entry where its other sections put it.
- `symbols.txt`: set each variable's exact size (`kind:bss(size=0x…)`) so objdiff sees the same size on both sides. Leave alias symbols in place for dsd.
- `relocs.txt`: no changes expected. If an alias has to go (open question 4), rewrite the relocations to it as `to:VARIABLE add:OFFSET` and change its C users to match.

### R6. Commands

```
python3 tools/bss.py scan MODULE [--verbose]     # variables, aliases, filler, runs; writes nothing
python3 tools/bss.py plan MODULE [--range LO-HI] # the files it would write and where
python3 tools/bss.py plan MODULE [--range LO-HI] --write
```

- `--range` takes part of a module, so main can go in over several commits.
- `--write` must leave a tree where `make release` passes. If the build does not match, it says which module's hash failed. The fix is a change to the tool or to the config it reads, not a hand edit of the generated C.
- Running it twice in a row changes nothing.

### R7. Tests and docs

- `tools/tests/test_bss.py`, run by `make test`, with no ROM: variable building (aliases, filler, declared sizes) and the run-minimising split, on made-up symbol lists. When the compiler is present, compile one generated file and check its section order.
- `docs/workflow.md`: how to run it, and how to move a variable later (into a topic file, or with a new name through `fn.py rename`).
- Issue 0082: progress per module.

## What the tool does where the requirements leave a choice

`tools/bss.py` has these rules in its docstring; the same config, `src/`, `include/`, `build/fnindex.json`, `build/dsindex.json` and compiler give the same output, and `tools/tests/test_bss.py` runs `plan --show` twice on each overlay and on arm7 to check that. Each run has its own hash seed, so the test also catches a choice that depends on the order of a Python set: the first version picked among hosts with equal votes that way, and arm7's plan changed from one run to the next.

- Sort order (background): for uninitialized variables the compiler's heapsort starts from definition order, not the reverse. In a file with both kinds it starts from the uninitialized ones in definition order, then the initialized ones (zero-initialized included) in reverse order. `tools/mwcc_order.py` takes a flag per object; `test_data_order.py` checks all three cases.
- R1: the declarations come from the `extern` statements in `src/` and `include/`, read by the tool. dsindex's regex leaves out function pointers (`extern int (*gAiUnloadTest)(...)`), which made the C conflict with the real declaration. dsindex still gives the struct sizes; a size the tool cannot work out from the text comes from compiling `sizeof` in the declaring file.
- R2: an array declared with no count gets as many whole elements as fit before the next symbol, at least one; symbols inside its first element are aliases. Filler stretches lie between two variables, so the cut of each one is independent of the others: fewest places where the size goes down, then fewest pieces (no table over the whole list is needed). Less than 32 bytes of filler before a 32-aligned variable becomes `__attribute__((aligned(32)))` on that variable when that adds no descent, since it adds no placeholder.
- R3: a host's code must be in the same part of the module as the run: ARM7 has three parts (crt0, main RAM, WRAM), and `tools/arm7.py` wants each file in one of them. Neighbouring runs share a file when they go to the same host, or the same directory (`include/bss_run.h` puts runs past the first in `.bss.runN`). A run whose variables nothing uses joins the file before it. A run with no host joins a hosted file before it in the same directory, and a hosted run takes in the unhosted file before it when both are in its directory. A host must also compile to code only (data of its own would join the sort), declare the variables with their sizes, and compile with the definitions in order; otherwise the run gets a new file. New files are named `<directory>_bss.c` in a topic directory, `bss_<address>.c` at the module level (`ovNNN_bss_<address>.c` in an overlay).
- R4: a definition copies a declaration without `extern`: the host's own (which it replaces), else a header's (which the new file includes), else the most common one in a `.c` file, with a comment naming the files that also declare it. A type no header defines gives a `u8` array with a comment. Declarations were not moved into headers: many variables are declared in several files with different types (`data_ov004_022bf158` has three), so a shared header would need those files changed by hand first. When a host's own `extern` statements cannot be replaced (a type defined further down), the definitions go at the end of the file and the `extern` statements stay. A comment of whole lines just above a replaced `extern` goes above that variable's definition, so it is not left above the next function prototype. When two headers define a type (ARM9's `struct OSThread` in `sdk/os.h` is 0x94 bytes, ARM7's in `sdk/os_arm7.h` 0x78), a definition copied from a `.c` file takes the header that file includes, and the compile check compares each variable's size in the object with the ROM's, not only its place.
- R6: `plan --show` prints the C each file would get.
- ARM7 (step 4): `bss.py lcf --arm7 build/arm7/arm7.lcf` runs in the ARM7 link script step, after `arm7.py lcf` has put the real addresses back. Its lines name objects and symbols only (`file.o(.bss.run1)`, `ALIAS = VARIABLE + 0x8;`), so the moved addresses of the stand-in ROM do not matter to it. `.sbss` is the 24 bytes between ARM7 WRAM's `.data` and `.bss` (`docs/tool-issues.md`); mwccarm puts nothing there on its own, so every run of a `.sbss` range goes between `#pragma section SBSS begin` (the first) or `SBSS_RUNn` (the others) and `end`, and the linker script places `.sbss.runN` after the file's `.sbss`. `tools/tests/test_data_order.py` checks the size sort with the ARM7 flags as well; it gives the same order.
- Not done: a `--range` that cuts a run puts the two halves in separate files.

## Order of work

1. Pilot on ov004 (8 symbols, 608,288 bytes). Answer the open questions below and write the answers into this file.
2. ov000, ov002, ov003, ov005, ov006.
3. main, in address ranges of a size that reviews well.
4. arm7, including `.sbss`.

Each step: `make release` passes, decomp.dev's linked data goes up by the module's `.bss`, and objdiff scores the new units 100% matched.

## What the ov004 pilot found

`python3 tools/bss.py plan ov004 --write` put both of ov004's runs into existing files, with no new file: `data_ov004_022bf140` (12 bytes) into `ov004_init3d.c`, its only user, and the other seven variables (608,276 bytes) into `ov004_alloc_state.c`, which uses all seven. The ROM matches, and objdiff scores both units 100%: decomp.dev's linked and matched data went from 188,932 to 797,220 bytes (6.02% to 25.4%). Four variables that only their file names became `static`; objdiff matches them by name all the same. Run again, the tool finds nothing to do, and run from a clean tree it writes the same bytes.

The questions the pilot had to answer, and how each was tested:

1. **A `.bss` range in a file with `.text`: yes, while the order agrees.** dsd sorts the files of each section by address and links them in one order (`sort_files` in dsd's `config/delinks.rs`). Giving `ov004_init3d.c` (`.text` before `ov004_alloc_state.c`) the `.bss` after `ov004_alloc_state.c`'s made `dsd delink` stop with "Link order cycle detected while sorting delink files". In the right order it builds, as the pilot shows. `bss.py` runs the same check (`link_order_ok`) before it hosts a run and makes a new file when the check fails.
2. **More than one run in a file: yes.** The compiler sorts all of a file's variables together, whatever their section, but each section keeps its variables in that sorted order. So run N goes in its own section through `#pragma define_section` (`include/bss_run.h`), and the linker script lists `file.o(.bss.runN)` after `file.o(.bss)`. Tested by building ov004's two runs in one scratch file: the ROM matched. The section name matters to objdiff: `.bss_run1` linked and matched the ROM, but objdiff left its variables unmatched (matched data unchanged); `.bss.run1` scored 100%, so objdiff reads `.bss.N` as `.bss`. mwldarm puts a section the script does not name at address 0 without an error, so `bss.py lcf` adds the lines from the sizes in `symbols.txt`. `test_bss.py` compiles a file of three runs. The pilot did not need it: its two runs have no users in common.
3. **Alignment: `__attribute__((aligned(32)))` works.** mwccarm 2.0-72 gives the variable's section alignment 32, and in a test link with `ALIGNALL(4)` mwldarm placed it on a 32-byte boundary after a 4-byte variable. `__declspec(align(32))` is a syntax error; `#pragma align(32)` changes nothing. main has 8 such variables, arm7 2; ov004 none.
4. **Aliases count against the unit unless their size is 0.** ov004 has no alias, so the test added one to the pilot by hand and reverted it: `data_ov004_022bf15a`, inside `data_ov004_022bf158`. With `kind:bss` and no size, dsd gives it the 2 bytes up to the next symbol, our object has nothing there, and objdiff scored `ov004_alloc_state`'s `.bss` 99.99967% and counted none of its 608,276 bytes as matched. With `kind:bss(size=0x0)` the unit scored 100%. So the tool writes aliases with size 0 and leaves `relocs.txt` alone; no alias has to go.

## What the arm7 step found

`python3 tools/bss.py plan arm7 --write` wrote 107 variables (98 named, 9 placeholders, 26 aliases, 2 aligned to 32) in 36 runs: 23 existing files took runs, and 5 new files hold the rest (`src/arm7/sdk/os/os_bss.c`, `os/os_bss_03807888.c` for `.sbss`, `os/os_bss_0380afe8.c`, `arm7_bss_03807aac.c`, `ctrdg/ctrdg_bss.c`). The ROM matches, every ARM7 unit's `.bss` and `.sbss` scores 100%, and decomp.dev's linked data went from 797,220 to 817,752 bytes (25.4% to 26.1%). Run again it finds nothing to do, and run from a clean tree it writes the same bytes.

- objdiff reads `.sbss.run1` as `.sbss`, as it does `.bss.runN`: the `.sbss` file has two runs and scores 100%.
- The first build moved all of ARM7 WRAM from `gBootThread_Arm7` on by 0x1c bytes. The new file had included ARM9's `sdk/os.h`, whose `OSThread` is 0x94 bytes, instead of `sdk/os_arm7.h` (0x78). The order check passed, since both threads had the same wrong size; the size check that came out of it would have stopped the write.
- With the right header, files in `src/arm7/sdk/os/` compile with the definitions and became hosts: the WRAM `.bss` of the OS library went into five existing files instead of one new file.
- The IRQ slot table at 0x0380788c (`data_arm7_0380788c[]`) runs on from `.sbss` into `.bss`. The tool gives it one element in `.sbss` and keeps the timer entries (`data_arm7_038078bc`) as their own variable in `.bss`; one object over both would need the `.sbss` workaround in `docs/tool-issues.md` to go first.

## Fixes before the ARM9 rollout

A trial wrote all of ARM9's `.bss` (main, ov000, ov002, ov003, ov005, ov006) with `plan --write`; with two hand fixes it passed `make release`. Each case below was checked again on mainline 3a0dcf832 (after the arm7 step) with `plan main --show` and one `plan main --write`, then the tree was reset. Addresses are in main unless said otherwise.

1. **`aligned(32)` on a pointer.** `src/sdk/mb/mb_pieces_is_complete.c` gets `DlPlayWorkerArea* data_022acf00 __attribute__((aligned(32)));`. mwccarm 2.0-72 ignores the attribute after a pointer or function pointer declarator: the section keeps alignment 4. Written before the declaration (`__attribute__((aligned(32))) DlPlayWorkerArea* data_022acf00;`, also after `static`) it gives 32, for every type tried (pointer, function pointer, `u8` array, `u16` 2-D array, struct, `u32`, in `.bss` and in `.bss.run1`). The ROM still matched only because the 24 filler bytes 0x022acee8-0x022acf00 belong to no file: `fill` turns them into the attribute, so they fall between `mb_parent_init_wm.c` (ends 0x022acee8) and `mb_pieces_is_complete.c` (starts 0x022acf00) and stay in gap unit `_dsd_gap@main_19`. A second `plan main` after the write then plans `src/game/main/bss_022acee8.c` for them, against R6 ("running it twice changes nothing"). Of main's 8 aligned variables, this is the only pointer and the only one at the start of a file; the other 7 are inside a file, where the file's range covers the filler.
   - Required: `definition` puts the attribute first (`[static ]__attribute__((aligned(32))) TYPE NAME;`). `order_errors` also fails when a variable with `align32` sits in a section whose alignment is below 32 ("data_022acf00: section aligned to 4, needs 32"). Filler that `fill` drops for an aligned variable at the start of a file goes to the end of the file before it in the plan when that file ends where the filler starts (here `mb_parent_init_wm.c`: `.bss` end 0x022acf00); when no planned file ends there, the tool stops and names the variable.
   - Test: `test_bss.py` compiles a file with an aligned pointer and an aligned `u8` array (one in `.bss`, one in `.bss.run1`) and checks alignment 32 for both; a made-up gap where the aligned variable starts a group checks that the group before it ends at the aligned address. On the ROM: after `plan main --write`, `plan main` prints "nothing to write" and the report has no `.bss` in any main gap unit.

2. **Aliases past a variable's declared size.** dsd gives these aliases size 0, but objdiff gives a size-0 symbol that no other symbol covers the bytes up to the next symbol, and our object has nothing there. `src/game/main/screen/screen_bss.c` (71,004 bytes) scores 99.98733% and `src/game/main/link/link_bss.c` (4,112 bytes) 99.92681%, so neither counts as matched data. Still five on mainline:

   | Alias | Variable | Declared | Alias offset | Declared in |
   |---|---|---:|---:|---|
   | `data_0216e5b2` | `data_0216e5b0` (`u16`) | 2 | 0x2 | `screen/screens_init_contexts.c` |
   | `data_0216e5b6` | `data_0216e5b4` (`s16`) | 2 | 0x2 | same |
   | `data_0216e5c9` | `data_0216e5c0` (`Unk_02080440_5c`) | 9 | 0x9 | same |
   | `data_0216f94e` | `data_0216f5f4` (`struct FontState`) | 0x35a | 0x35a | same |
   | `data_0229ca51` | `data_0229ca50` (`u8`) | 1 | 0x1 | `link/link_read_unflagged_packet.c` |

   - Required: before writing anything, `plan` (with or without `--write`) stops when a variable in the planned range has an alias at an offset at or past its declared size, and lists each one as in the table: alias, variable, declared size, offset, the files that declare the variable. No workaround (no size, no padding variable). `scan` lists the same lines as warnings and goes on.
   - Test: `build_items` on made-up symbols with a declared 2-byte variable and an alias at +2 returns the alias in a list `plan` refuses; one at +1 does not. On the ROM: `plan main --show` stops with the five lines above until the hand fixes below land.
   - Hand fixes, for a decompilation commit before main's `.bss` (each compiled here against the current file: same `.text`, relocations and symbols; the aliases stay, now inside the declared size, and the placeholders after the variables go away):
     - `data_0216e5b0` and `data_0216e5b4`: `Screens_InitContexts` stores `&data_0216e5b0` in `unk_40` for the main screen and `&data_0216e5b2` for the sub screen, the same for `scriptSlotCurrent`. One value per screen: `u16 data_0216e5b0[2]`, `s16 data_0216e5b4[2]`.
     - `data_0216e5c0`: `unk_5c[0]` and `unk_5c[1]` get `&data_0216e5c0` and `&data_0216e5c9`, swapped for the sub screen. `Unk_02080440_5c data_0216e5c0[2]` (18 bytes, up to `data_0216e5d4`; placeholder `data_0216e5cc` goes).
     - `data_0216f5f4`: one `FontState` per screen, the second at `data_0216f94e` and ending at `data_0216fca8`. `struct FontState data_0216f5f4[2]` (0x6b4 bytes; placeholder `data_0216f950` goes).
     - `data_0229ca50`: `Link_PopUnflaggedPacket` copies 24 bytes into it (`Mem_CopyBytes(..., dest, 24)`), and `Link_ReadUnflaggedPacket` returns the packet past its header byte. `u8 data_0229ca50[24]` (up to `data_0229ca68`; placeholder `data_0229ca54` goes).

3. **`--range` summary.** "runs before cutting filler" counts the whole module: every range of main prints 132. It must count only the variables that start in the range, as "after" does. Correct numbers (before, after): 0x0216e020-0x0218ff50 23, 19; 0x0218ff50-0x022900b4 16, 16; 0x022900b4-0x0229e160 25, 24; 0x0229e160-0x022a9870 33, 33; 0x022a9870-0x022ad560 35, 32. The ranges add up to the whole module (132, 124). Test: `make_plan` on a made-up module of two gaps with a range over one of them returns the runs of that gap only.

4. **Same output under any hash seed.** Holds on mainline: `plan main --show` gives byte-identical output under `PYTHONHASHSEED=1`, `2` and `3` (2,598 lines), and the trial's host changes between runs are gone (ties follow fnindex order). The three hosts that differ from the trial are now the same on every run. Test: `test_twice` runs `plan MODULE --show` with `PYTHONHASHSEED=1` and then `2` set in the environment (not left random), for every module including main (main takes about 7 seconds a run). It needs `build/fnindex.json`, which needs a build and so a ROM; without them it skips as now. What runs without a ROM: a `best_host` test on a made-up `Users` with two files of equal votes, run in two subprocesses with different seeds, that must pick the file first in fnindex order both times.

5. **Other defects found.** None beyond the above: one `plan main --write` on mainline matched the ROM, and the report shows `.bss` below 100% only in `screen_bss` and `link_bss` (item 2) and `.bss` left in a main gap unit only at 0x022acee8 (item 1).

## Open decisions for sturmds

Numbers from `plan --show` on mainline, ARM9 modules with `.bss` left.

(a) **Runs that interleave in one file.** Now: main 66 files (34 hosted, 32 new) with 122 `#pragma section BSS_RUNn` pairs, 131 for ARM9 without ov003, 144 with it. Worst files: `screen/screen_bss.c` 11 runs and 27 pairs, `bss_0218ff50.c` 16 runs and 26 pairs, `bss_022a7a88.c` 7 runs and 15 pairs. Options:
   - Search for a definition order with fewer switches. At least one pair per run past the first: 58 for main. The trial's run-grouping test kept the compiler's order in 8 of 28 multi-run files, so a search is needed and may not reach the bound.
   - One file per run once a file passes a limit. Limit 4: 6 files of main pass it, 38 more files, 39 pairs left. Limit 8: 3 files, 31 more files, 54 pairs left.
   - Proposed (sturmds, 2026-10-13): a file never holds variables of two runs, but a run may be cut into several files ("N files to 1 run, never shared"). A slice of an ascending run is ascending, so no ARM9 file needs a `BSS_RUNn` pragma. A prototype of the rule below (scratch, not committed) gives:

     | Module | Runs | Files | Hosted | New | Largest new files (bytes) |
     |---|---:|---:|---:|---:|---|
     | main | 124 | 188 | 134 | 54 | 262,316; 193,764; 135,308; 130,960 |
     | ov000 | 4 | 4 | 3 | 1 | 128 |
     | ov002 | 3 | 4 | 2 | 2 | 180, 28 |
     | ov003 | 7 | 7 | 6 | 1 | 1,636 |
     | ov005 | 2 | 12 | 12 | 0 | |
     | ov006 | 2 | 4 | 3 | 1 | 4 |

     main goes from 66 files (34 hosted, 32 new) to 188 (134 hosted, 54 new); its hosted bytes go from 53,408 to 45,320 (hosts of several runs keep one slice each), and 1,262,488 bytes stay in new files. The 1 MB at 0x0218ff50 becomes 16 new files, one per run, since nothing there finds a host.

     How `bss.py` chooses the cuts: (1) each named variable gets a preferred host: among its users' files that pass `host_allowed` and `declarations_fit`, the one the variable before it chose if it is among them, else the first in fnindex order; (2) placeholders, variables nothing uses and variables with no allowed host join the slice before them in the run, else the one after; (3) cut where the preferred host changes, and merge neighbouring slices with the same host; (4) a file takes one `.bss` range, so a host met again later loses that later slice; (5) check the link order (`link_order_ok`) and the compile (`render`) in address order, and when a host fails, its slice tries to join the hosted slice before it in the same run, then the one after, each checked again; (6) neighbouring slices left with no host in one run make one new file. Everything goes in address order with ties by fnindex order, so the output stays the same under any hash seed.

     What changes: `group_runs` works on slices instead of runs, and `MAX_RUNS[".bss"]` becomes 1 for ARM9. `include/bss_run.h` keeps its pragmas, since arm7's files use `.bss.runN` and `.sbss.runN` (ov004's two runs are in two files and use none). `bss.py lcf` keeps placing `.bss.runN` for arm7; for ARM9 it writes only alias lines. Whether arm7 is redone to the same rule is open.

(b) **ov003's host. Decided (sturmds): its `.bss` stays in `ov003_exit.c`**, no new `ov003_bss.c` (7 runs, 19 variables, 16,672 bytes; 13 pragma pairs as planned now), landed apart from this work. This does not fit the rule proposed in (a): `ov003_exit.c` uses only `data_ov003_02365b0c` and `data_ov003_02365b10`, both in run 1 (0x02365b0c-0x02365cf4 with `data_ov003_02365b14`, 488 bytes), so under that rule it holds that slice at most. The prototype puts the other runs in `random.c`, `ov003_begin3d.c` (14,400 bytes), `ov003_draw_items.c`, `ov003_update_rows_and_palette.c`, `ov003_create_anchored_sprite.c` and one new file for 0x02369534-0x02369b98. ov003 then needs an exception to the rule, or the decision changes.

(c) **The 1 MB `bss_0218ff50.c`. Decided (sturmds): generated definitions**, specified in the next section. Now planned as one file: 35 variables, 1,048,932 bytes, 16 runs, 26 pragma pairs; 31 of the variables are `u8` arrays (30 sized up to the next symbol, 1 placeholder), and only `gGameHeap`, `data_0218ff54`, `gOwnerInfo` and `data_0228ffa8` have types. Under (a)'s rule it becomes 16 new files, one per run.

## Generated definitions (decision c)

A file whose definitions are mostly address-named `u8` arrays is not written into `src/`. `bss.py` generates its definitions at build time into `build/gen/`, the way `maps.py pack` writes `build/gen/maps/`, and the committed `.c` file only includes them. `.bss` has no bytes, so this needs no ROM and no game data: only committed config and headers.

- **Which files.** A new file of the plan (not a host) whose variables are more than half without a type: placeholders, and variables with no `extern` in a header, which get `u8 NAME[SIZE]` today. First the 16 files of 0x0218ff50-0x022900b4. `plan --write` marks such a file by adding its path to `config/AWRE/bss_gen.txt` (one path per line, sorted); `plan` prints "generated" next to it. Hosts are never generated: their definitions replace `extern` statements in code that people edit.
- **What goes in the generated file.** The whole slice, typed variables too. The compiler orders a file's variables by a sort that starts from definition order, so the definitions of one file are written in one place and one order (`mwcc_order.definition_order`); splitting them between the `.c` and the generated file would make that order depend on where the `#include` sits.
- **The command.** `bss.py gen OUT_DIR` writes `OUT_DIR/bss/<stem>.inc.c` for every path in `bss_gen.txt`. For each file: its range from the module's `delinks.txt`; the variables in it, with addresses and sizes, from `symbols.txt` (the `size=` that `plan --write` wrote; aliases have size 0 and stay in the linker script); a variable's type from the `extern` statements in `include/` only (a header the generated file can include), else `u8 NAME[SIZE]`; `__attribute__((aligned(32)))` first when the address is 32-aligned and the bytes before it in the file belong to no symbol. The generated file starts with a "generated by tools/bss.py gen; do not edit" comment, then `#include "types.h"`, the headers of the types used (sorted), and the definitions in definition order, one per line. It does not read `build/fnindex.json`, `build/dsindex.json` or compile anything: those need a build first, and this file is part of the build. A type's size is not checked here; a wrong one moves the variables after it, and the ROM check fails.
- **The committed `.c`.** One line plus the include, formatted like any file:

  ```c
  /* Definitions generated from config/AWRE/arm9/symbols.txt (docs/bss-tool.md). */
  #include "bss/bss_0218ff50.inc.c"
  ```

  `delinks.txt` gives it its `.bss` range and `complete`, as for any new file. objdiff and decomp.dev see it as a normal unit.
- **The build step.** `tools/configure.py` adds a rule `bss_gen` (`$python tools/bss.py gen build/gen`) with outputs `build/gen/bss/<stem>.inc.c` for each listed path, and inputs `config/AWRE/bss_gen.txt`, the `delinks.txt` and `symbols.txt` of each module that has a listed file, every header in `include/`, `tools/bss.py` and `tools/mwcc_order.py`. It writes a file only when its text changes, and the rule sets `restat = 1`, so a header edit recompiles nothing that did not change. Each listed `.c` gets its `.inc.c` as an implicit input, as `data_inc_users` does for `data_inc.py` outputs. `-i build/gen` is already in both compilers' flags. `make format` leaves `build/gen/` alone, so the text must come out in its final form.
- **Moving a variable out.** Two steps, each its own commit:
  1. A real type and name: `fn.py rename` changes the name in `symbols.txt` and `src/`, and the agent adds `extern TYPE NAME;` to a header (with the struct, if new). The next build writes the definition with that type and includes the header; the committed `.c` does not change. When every variable of the file has a header type, `bss.py` drops the file from `bss_gen.txt` on the next `plan --write` and writes the definitions into the `.c` as for any new file.
  2. Into a code file that uses it: `bss.py move NAME FILE` takes a variable at either end of a generated file's range and gives it to FILE's `.bss` range (FILE must pass `host_allowed`, the link-order check and the compile check, and must already end or start next to it in the same run). A variable in the middle of the range cuts the slice in three: the variable's file and two generated files, `<stem>.inc.c` and `<stem>_<address>.inc.c`. Each piece still holds part of one run only, so (a)'s rule holds.
- **Tests.** In `test_bss.py`, no ROM: `gen` on a made-up `delinks.txt`, `symbols.txt` and header into a temporary directory, twice, under `PYTHONHASHSEED=1` and `2`, and once more after deleting the directory: the three outputs are byte-identical. The generated text compiled with the stub gives the variables in address order (`order_errors`). A variable given a header type changes only its own line and adds the include. On the ROM: `rm -rf build/gen/bss && make` rebuilds the same `.inc.c` bytes (compare `sha1sum` before and after) and `make check` passes; `plan main --write` run twice changes nothing the second time.

## Not in scope

- `.data` and `.rodata` (steps 3 and 4 of issue 0082).
- Names and types for placeholder variables. The tool keeps address names; working out what they are is decompilation work.
