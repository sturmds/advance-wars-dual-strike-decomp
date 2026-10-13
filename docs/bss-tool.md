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

## Not in scope

- `.data` and `.rodata` (steps 3 and 4 of issue 0082).
- Names and types for placeholder variables. The tool keeps address names; working out what they are is decompilation work.
