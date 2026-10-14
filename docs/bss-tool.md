# tools/bss.py: requirements

Step 2 of issue 0082 (GitHub #1): define every `.bss` variable in C, so that the `.bss` in dsd gap units (1,955,164 bytes, 62% of all data) links from source files marked `complete`. This file says what the tool must do; review it before the tool is written.

## Background

- `.bss` is RAM that starts out zeroed: by crt0 for main, by the overlay loader for an overlay (how the SDK works in general; not yet read in this ROM's code). mwccarm writes it as `NOBITS` sections, so it has no bytes in the ROM. The ROM records only its bounds: main's `bss_start`/`bss_end` in the module parameters, and each overlay's `bss_size` in the overlay table (`extract/arm9/arm9.yaml`, `extract/arm9_overlays/overlays.yaml`).
- What `make check` can catch: an error that moves an address the ROM stores. Those are the pointers to `.bss` in code and data (literal pools, with relocations) and the bounds above. It cannot catch a wrong split between two such addresses (`u8 x[8]` against two `u32`), equal-size variables in swapped order, or a small error in the last variables of main, whose end the linker script rounds up to 32. objdiff does not catch these either, since its target sizes come from the `symbols.txt` the tool writes. So a matching build pins the addresses that something points at; the sizes and types between them come from our declarations, or are guesses for placeholders.
- mwccarm 2.0 puts each data or `.bss` variable of a file in its own section and orders the sections by a heapsort on size, starting from the reverse of the definition order (`docs/workflow.md`, "Several data objects in one file"). This holds for globals, `static` variables, function statics and zero-initialised globals. The linker's `ALIGNALL(4)` aligns each one to 4.
- So the `.bss` of one original file is a run of ascending sizes in the ROM. Where the size goes down, a new original file starts. A run can still hold several original files when their sizes happen to keep ascending.
- One section of a file can hold only variables that form one ascending run. The pilot found a way to put several runs in one file (open question 2), but the files now hold a slice of one run each (decision a).

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

`tools/bss.py` has these rules in its docstring; the same config, `src/`, `include/`, `build/fnindex.json`, `build/dsindex.json` and compiler give the same output, and `tools/tests/test_bss.py` runs `plan --show` on every module under `PYTHONHASHSEED=1` and `2` to check that. The test catches a choice that depends on the order of a Python set: the first version picked among hosts with equal votes that way, and arm7's plan changed from one run to the next.

- Sort order (background): for uninitialized variables the compiler's heapsort starts from definition order, not the reverse. In a file with both kinds it starts from the uninitialized ones in definition order, then the initialized ones (zero-initialized included) in reverse order. `tools/mwcc_order.py` takes a flag per object; `test_data_order.py` checks all three cases.
- R1: the declarations come from the `extern` statements in `src/` and `include/`, read by the tool. dsindex's regex leaves out function pointers (`extern int (*gAiUnloadTest)(...)`), which made the C conflict with the real declaration. dsindex still gives the struct sizes; a size the tool cannot work out from the text comes from compiling `sizeof` in the declaring file.
- R2: an array declared with no count gets as many whole elements as fit before the next symbol, at least one; symbols inside its first element are aliases. Filler stretches lie between two variables, so the cut of each one is independent of the others: fewest places where the size goes down, then fewest pieces (no table over the whole list is needed). Less than 32 bytes of filler before a 32-aligned variable becomes `__attribute__((aligned(32)))` on that variable when that adds no descent, since it adds no placeholder.
- R3: files hold slices of one run, cut by the rule in decision (a) below. A host's code must be in the same part of the module as the slice: ARM7 has three parts (crt0, main RAM, WRAM), and `tools/arm7.py` wants each file in one of them. A host must also compile to code only (data of its own would join the sort), declare the variables with their sizes, and compile with the definitions in order; otherwise its slice joins a hosted neighbour or gets a new file. New files are named `<directory>_bss.c` in a topic directory, `bss_<address>.c` at the module level (`ovNNN_bss_<address>.c` in an overlay). arm7 was written before the rule, with neighbouring runs sharing a file in `.bss.runN` and `.sbss.runN` sections; those files stay as they are, and `bss.py lcf` still places their sections.
- R4: a definition copies a declaration without `extern`: the host's own (which it replaces), else a header's (which the new file includes), else the most common one in a `.c` file, with a comment naming the files that also declare it. A type no header defines gives a `u8` array with a comment. Declarations were not moved into headers: many variables are declared in several files with different types (`data_ov004_022bf158` has three), so a shared header would need those files changed by hand first. When a host's own `extern` statements cannot be replaced (a type defined further down), the definitions go at the end of the file and the `extern` statements stay. A comment of whole lines just above a replaced `extern` goes above that variable's definition, so it is not left above the next function prototype. When two headers define a type (ARM9's `struct OSThread` in `sdk/os.h` is 0x94 bytes, ARM7's in `sdk/os_arm7.h` 0x78), a definition copied from a `.c` file takes the header that file includes, and the compile check compares each variable's size in the object with the ROM's, not only its place.
- R6: `plan --show` prints the C each file would get, and for a generated file the text `bss.py gen` will write.
- ARM7 (step 4): `bss.py lcf --arm7 build/arm7/arm7.lcf` runs in the ARM7 link script step, after `arm7.py lcf` has put the real addresses back. Its lines name objects and symbols only (`file.o(.bss.run1)`, `ALIAS = VARIABLE + 0x8;`), so the moved addresses of the stand-in ROM do not matter to it. `.sbss` is the 24 bytes between ARM7 WRAM's `.data` and `.bss` (`docs/tool-issues.md`); mwccarm puts nothing there on its own, so every run of a `.sbss` range goes between `#pragma section SBSS begin` (the first) or `SBSS_RUNn` (the others) and `end`, and the linker script places `.sbss.runN` after the file's `.sbss`. `tools/tests/test_data_order.py` checks the size sort with the ARM7 flags as well; it gives the same order.
- A `--range` that cuts a run puts the two halves in separate files, which the rule in (a) allows. main's five ranges below cut no run.

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

A trial wrote all of ARM9's `.bss` (main, ov000, ov002, ov003, ov005, ov006) with `plan --write`; with two hand fixes it passed `make release`. Each case below was checked again on mainline 3a0dcf832 (after the arm7 step) with `plan main --show` and one `plan main --write`, then the tree was reset. Addresses are in main unless said otherwise. All four are now in the tool.

1. **`aligned(32)` on a pointer.** `src/sdk/mb/mb_pieces_is_complete.c` got `DlPlayWorkerArea* data_022acf00 __attribute__((aligned(32)));`. mwccarm 2.0-72 ignores the attribute after a pointer or function pointer declarator: the section keeps alignment 4. Written before the declaration (`__attribute__((aligned(32))) DlPlayWorkerArea* data_022acf00;`, also after `static`) it gives 32, for every type tried (pointer, function pointer, `u8` array, `u16` 2-D array, struct, `u32`, in `.bss` and in `.bss.run1`). The ROM still matched only because the 24 filler bytes 0x022acee8-0x022acf00 belonged to no file and stayed in gap unit `_dsd_gap@main_19`; a second `plan main` then planned `src/game/main/bss_022acee8.c` for them, against R6.
   - Done: `definition` puts the attribute first (`[static ]__attribute__((aligned(32))) TYPE NAME;`), and `order_errors` fails when a variable aligned to 32 sits in a section aligned to less ("data_022acf00: section aligned to 4, needs 32"). Filler left out for an aligned variable at the start of a file goes to the end of the file before it in the plan (`add_tails`), which must end where the filler starts; otherwise the tool stops and names the variable. Under the slicing rule `data_022acf00` starts `mb_task_end.c`, and `mb_parent_init_wm.c`'s range ends at 0x022acf00.
   - Tests: `test_bss.py` compiles a file with an aligned pointer in `.bss` and an aligned `u8` array in `.bss.run1` and checks alignment 32 for both; `add_tails` on a made-up gap ends the slice before at the aligned address, and stops when no slice ends there. On the ROM: after `plan main --write`, `plan main --write` prints "nothing to write" and no main gap unit has `.bss`.

2. **Aliases past a variable's declared size.** dsd gives these aliases size 0, but objdiff gives a size-0 symbol that no other symbol covers the bytes up to the next symbol, and our object has nothing there. The trial's `screen/screen_bss.c` (71,004 bytes) scored 99.98733% and `link/link_bss.c` (4,112 bytes) 99.92681%, so neither counted as matched data. There were five:

   | Alias | Variable | Declared | Alias offset | Declared in |
   |---|---|---:|---:|---|
   | `data_0216e5b2` | `data_0216e5b0` (`u16`) | 2 | 0x2 | `screen/screens_init_contexts.c` |
   | `data_0216e5b6` | `data_0216e5b4` (`s16`) | 2 | 0x2 | same |
   | `data_0216e5c9` | `data_0216e5c0` (`Unk_02080440_5c`) | 9 | 0x9 | same |
   | `data_0216f94e` | `data_0216f5f4` (`struct FontState`) | 0x35a | 0x35a | same |
   | `data_0229ca51` | `data_0229ca50` (`u8`) | 1 | 0x1 | `link/link_read_unflagged_packet.c` |

   - Done: `build_items` returns the unaligned aliases that lie at or past their variable's declared size (and inside its 4-byte padding; one further on is in a placeholder). `plan`, with or without `--write`, lists them as in the table and stops before writing anything; `scan` prints the same lines as a warning and goes on. No workaround in the tool.
   - Test: `build_items` with a declared 2-byte variable and an alias at +2 returns it; one at +1 does not.
   - Hand fixes, in a decompilation commit before main's `.bss` (`fn.py diff` still 100% for both functions; the aliases stay, now inside the declared size, and the placeholders after the variables go away):
     - `data_0216e5b0` and `data_0216e5b4`: `Screens_InitContexts` stores `&data_0216e5b0` in `unk_40` for the main screen and `&data_0216e5b2` for the sub screen, the same for `scriptSlotCurrent`. One value per screen: `u16 data_0216e5b0[2]`, `s16 data_0216e5b4[2]`.
     - `data_0216e5c0`: `unk_5c[0]` and `unk_5c[1]` get `&data_0216e5c0` and `&data_0216e5c9`, swapped for the sub screen. `Unk_02080440_5c data_0216e5c0[2]` (18 bytes, up to `data_0216e5d4`; placeholder `data_0216e5cc` goes).
     - `data_0216f5f4`: one `FontState` per screen, the second at `data_0216f94e` and ending at `data_0216fca8`. `struct FontState data_0216f5f4[2]` (0x6b4 bytes; placeholder `data_0216f950` goes).
     - `data_0229ca50`: `Link_PopUnflaggedPacket` copies 24 bytes into it (`Mem_CopyBytes(..., dest, 24)`), and `Link_ReadUnflaggedPacket` returns the packet past its header byte. `u8 data_0229ca50[24]` (up to `data_0229ca68`; placeholder `data_0229ca54` goes).

3. **`--range` summary.** "runs before cutting filler" counted the whole module: every range of main printed 132. Done: it counts only the variables that start in the range, as "after" does. Before the hand fixes (before, after): 0x0216e020-0x0218ff50 23, 19; 0x0218ff50-0x022900b4 16, 16; 0x022900b4-0x0229e160 25, 24; 0x0229e160-0x022a9870 33, 33; 0x022a9870-0x022ad560 35, 32. Test: `make_plan` on a made-up module of two gaps with a range over one of them returns the runs of that gap only.

4. **Same output under any hash seed.** `test_twice` runs `plan MODULE --show` with `PYTHONHASHSEED=1` and then `2` for every module, main included (a plan that stops on item 2 must stop the same way). It needs `build/fnindex.json`, which needs a build and so a ROM; without them it skips. Without a ROM: `best_host` on a made-up `Users` with three files, run in two subprocesses with different seeds, picks the allowed file first in fnindex order both times.

## Decisions (sturmds, 2026-10-13)

(a) **N files to 1 run, never shared.** A file holds a contiguous slice of exactly one ascending run; a run may be split across several files; no file holds variables of two runs. A slice of an ascending run is ascending, so no ARM9 file needs a `BSS_RUNn` pragma and the ARM9 link script gets no extra-run lines, only alias lines. `include/bss_run.h` keeps its pragmas, since arm7's files use `.bss.runN` and `.sbss.runN`; arm7 and ov004 were written before the rule and `plan` writes nothing for them. How `bss.py` chooses the cuts:

1. Each named variable gets a preferred host (`best_host`): among its users' files in the module that pass `host_allowed` and `declarations_fit`, the one the variable before it chose if it is among them, else the first in fnindex order.
2. Placeholders, variables nothing in the module uses and variables with no allowed host join the slice before them in the run, else the one after (`slice_run`). A run where no variable has a host is one slice with no host.
3. The run is cut where the preferred host changes; neighbouring slices with the same host are one slice.
4. A file takes one range per section, so a host met again later loses that later slice.
5. The link order (`link_order_ok`) and the compile (`render`) are checked in address order, each host against the ones accepted before it. A slice whose host fails, or that has none, tries to join the hosted slice before it in the same run, then the one after, each checked again (`assign_hosts`).
6. Neighbouring slices left with no host in one run make one new file.

Everything goes in address order with ties by fnindex order, so the output stays the same under any hash seed. The plan (main after the hand fixes in item 2):

| Module | Runs | Files | Hosted | New | Generated |
|---|---:|---:|---:|---:|---:|
| main | 121 | 182 | 134 | 48 | 43 |
| ov000 | 4 | 4 | 3 | 1 | 1 |
| ov002 | 3 | 4 | 2 | 2 | 2 |
| ov003 | 7 | 7 | 6 | 1 | 1 |
| ov005 | 2 | 12 | 12 | 0 | 0 |
| ov006 | 2 | 4 | 3 | 1 | 1 |

A hosted slice can hold variables its host does not use (step 2): `ov000/load_obj_gfx.c` takes 78 variables after the one it uses, since nothing else in ov000 uses them. `bss.py move` (below) is the way to take such variables out of a generated file; moving them out of a host is a hand edit of the host and `delinks.txt`.

(b) **ov003 follows the rule.** This replaces the earlier decision to keep all of ov003's `.bss` in `ov003_exit.c`. Under the rule `ov003_exit.c` would hold its slice at most, but it gets none: `random.c`, `ov003_load_obj_graphics.c`, `ov003_begin3d.c`, `ov003_draw_items.c`, `ov003_update_rows_and_palette.c` and `ov003_create_anchored_sprite.c` host six slices, and `ov003_bss_02369534.c` (generated) holds 0x02369534-0x02369b98.

(c) **Generated definitions** for files that are mostly address-named `u8` arrays, as specified in the next section. The 1 MB at 0x0218ff50 becomes 16 generated files, one per run.

## Generated definitions (decision c)

A file whose definitions are mostly address-named `u8` arrays is not written into `src/`. `bss.py gen` writes its definitions at build time into `build/gen/bss/`, the way `maps.py pack` writes `build/gen/maps/`, and the committed `.c` file only includes them. `.bss` has no bytes, so this needs no ROM and no game data: only committed config and headers.

- **Which files.** A new file of the plan (not a host) whose variables are more than half without a type: placeholders, and variables with no `extern` in a header (a variable declared only in `.c` files counts as without a type). `plan --write` adds its path to `config/AWRE/bss_gen.txt` (one path per line, sorted); `plan` prints "new, generated" next to it. Hosts are never generated: their definitions replace `extern` statements in code that people edit. `plan` compiles the text `gen` would write and checks it like any other file; if that fails (a header type of the wrong size), the file is written out as a normal new file instead.
- **What goes in the generated file.** The whole slice, typed variables too. The compiler orders a file's variables by a sort that starts from definition order, so the definitions of one file are written in one place and one order (`mwcc_order.definition_order`); splitting them between the `.c` and the generated file would make that order depend on where the `#include` sits.
- **The command.** `bss.py gen OUT_DIR` writes `OUT_DIR/bss/<stem>.inc.c` for every path in `bss_gen.txt` and removes the ones no longer listed. For each file: its range from the module's `delinks.txt`; the variables in it, with addresses and sizes, from `symbols.txt` (the `size=` that `plan --write` wrote; aliases have size 0 and stay in the linker script); a variable's type from the first `extern` statement in `include/` that declares it, else `u8 NAME[SIZE]`. An array declared with no count gets one when its element is a basic type or a pointer, else it is `u8 NAME[SIZE]`. `__attribute__((aligned(32)))` goes first when the address is 32-aligned and the bytes before it belong to no symbol. The generated file starts with a "Generated by tools/bss.py gen from ...; do not edit." comment, then `#include "types.h"`, the headers that declare the variables (sorted), and the definitions in definition order, one per line. It does not read `build/fnindex.json`, `build/dsindex.json` or compile anything: those need a build first, and this file is part of the build. A type's size is not checked here; a wrong one moves the variables after it, and the ROM check fails.
- **The committed `.c`.** One line plus the include:

  ```c
  /* Definitions generated from config/AWRE/arm9/symbols.txt (docs/bss-tool.md). */
  #include "bss/bss_0218ff50.inc.c"
  ```

  `delinks.txt` gives it its `.bss` range and `complete`, as for any new file. objdiff and decomp.dev see it as a normal unit.
- **The build step.** `tools/configure.py` adds a rule `bss_gen` (`$python tools/bss.py gen build/gen`) with outputs `build/gen/bss/<stem>.inc.c` for each listed path, and inputs `config/AWRE/bss_gen.txt`, the `delinks.txt` and `symbols.txt` of each module that has a listed file, every header in `include/`, `tools/bss.py` and `tools/mwcc_order.py`. It writes a file only when its text changes, and the rule sets `restat = 1`, so a header edit recompiles nothing that did not change. Each listed `.c` gets its `.inc.c` as an implicit input, as `data_inc_users` does for `data_inc.py` outputs. `-i build/gen` is already in both compilers' flags.
- **Moving a variable out.** Two steps, each its own commit:
  1. A real type and name: `fn.py rename` changes the name in `symbols.txt` and `src/`, and the agent adds `extern TYPE NAME;` to a header (with the struct, if new). The next build writes the definition with that type and includes the header; the committed `.c` does not change. When every variable of the file has a header type, the next `plan MODULE --write` writes the definitions into the `.c` as for a new file and drops it from `bss_gen.txt`.
  2. Into a code file that uses it: `bss.py move NAME FILE` gives a variable of a generated file to FILE. FILE must be `complete`, compile to code (and `.bss`) only, have its code in the same part of the module, declare the variable with its size, and keep the link order. When FILE has a range in the same section, it must end where the variable starts (the generated file's first variable) or start where it ends (its last), and the variables together must be ascending; FILE's definitions are taken out and written back with the new one in definition order, and the result is compiled and checked. When FILE has no `.bss`, any variable can go: one in the middle cuts the generated file in two, and the part after it becomes `<stem>_<address>.c` (address of the variable), listed in `bss_gen.txt`. A generated file left empty is removed. Each piece still holds part of one run only, so (a)'s rule holds. `move` then builds and runs `make check`, as `plan --write` does.
- **Tests.** In `test_bss.py`, no ROM: `gen` on a made-up `delinks.txt`, `symbols.txt` and header into a temporary directory, under `PYTHONHASHSEED=1` and `2`, and once more after deleting the directory: the three outputs are byte-identical. A variable given a header type changes only its own line. With the compiler, the generated text compiles and gives the variables in address order and the aligned one in a section aligned to 32 (`order_errors`). `take_definitions` (used by `move`) on a made-up file. On the ROM: `rm -rf build/gen/bss && make` rebuilds the same `.inc.c` bytes and `make check` passes; `plan MODULE --write` run twice changes nothing the second time.

## Aliases (issue 0083)

An alias makes the source say that two names share memory, and a size of 0 keeps objdiff from checking it. When the variable before it is declared too large, the alias hides a separate variable: `SoundWaveArc` ended in `u32 table[1]`, 4 bytes longer than the header the game reads into `gWaveArcHeader`, so the flag `gSoundStreamReady` after it became `gWaveArcHeader + 0x3c`. So every alias is listed, and new ones are checked before they are written.

`config/AWRE/aliases.txt` has one line per alias in the linker scripts (the size-0 symbols inside the `.bss`, `.data` or `.rodata` range of a `complete` file; `tools/data.py` follows the same rules, `docs/data-tool.md`):

```
MODULE SYMBOL = BASE+OFFSET GROUP
main data_0216e060 = data_0216e05c+0x4 unsorted
```

The group says what the alias is:

- `real FIELD`: a real address inside the variable, which code takes on purpose. FIELD names the element or field it lies on, as C after the base: elements and fields (`[64]`, `[0][1]`, `[0].pictures[2]`, `coRecords[0].setMode1`), then an optional byte count (`unk_348[0].value[1]+1`), or only a byte count (`+0x1000`) when the base's type does not describe the place: a compiler base, or a flat `u8[]`, `u32[]` or `void*[]` table standing for rows. Text after the first space is a note.
- `hidden`: a separate variable that the variable before it swallows. Fix it as `gSoundStreamReady` was fixed: shrink the type, give the symbol its size in `symbols.txt` and a definition in C, and drop the line.
- `unsorted`: not checked yet. The 200 aliases from before the list start this way; issue 0083 tracks the audit.

The checks:

- `bss.py plan` stops before writing anything when a new alias is not in the list, and when an alias lies on the last element of a `[1]` array that ends its variable's type (a variable-length tail, as in `SoundWaveArc`), unless the list gives it a `real` group. `scan` prints both as warnings and goes on.
- `tools/alias_check.py` compiles the file that defines each base again with mwccarm, with `&(BASE FIELD)` added, and compares the offset the relocation gives with OFFSET. A FIELD that does not compile fails too. So inserting a field before an alias, or changing the size of the elements it sits on, stops `make release` (`decomp_dev_report.py` runs it) with the list of aliases to move. An alias whose FIELD is or ends in a byte count is checked only for its path part; `alias_check.py --list` prints those, and `tools/shift_gaps.py` counts them as the `alias` row.
- `bss.py aliases` compares the list with the aliases that `symbols.txt` and `delinks.txt` give: each one listed once, with the same base and offset. `tools/decomp_dev_report.py`, the last step of `make release`, runs the same check and fails on a difference.

To add an alias, check what it is, add its line with the group, then run `plan`. To remove one (a hidden variable made real), drop its line in the same commit as the `symbols.txt` change.

## Not in scope

- `.data` and `.rodata` (steps 3 and 4 of issue 0082).
- Names and types for placeholder variables. The tool keeps address names; working out what they are is decompilation work.
