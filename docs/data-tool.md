# tools/data.py: .data and .rodata in C

Step 3 of issue 0082: define each initialised `.data` and `.rodata` object in C, so that it links from a file marked `complete`, as `tools/bss.py` does for `.bss` (`docs/bss-tool.md`). The tool reuses bss.py's code for the object list, aliases, runs, slicing and hosts, and `tools/data_inc.py` for the values. Game bytes never enter git (tenet 9): every value comes from the builder's ROM through an `inc.c` file (`docs/workflow.md`, "Data from the ROM").

## Commands

```
python3 tools/data.py scan MODULE [--verbose]       # objects, aliases, filler and runs; writes nothing
python3 tools/data.py plan MODULE [--range LO-HI]   # the files it would write, and where (--show: their C)
python3 tools/data.py plan MODULE [--range LO-HI] --write
python3 tools/data.py relocs MODULE                 # relocations in the module's data to check by hand
```

MODULE is `main`, `dtcm`, `ovNNN` or `arm7`. It needs `build/fnindex.json` and `build/dsindex.json` (`make fnindex dsindex`), the compiler from `make setup` and `orig/AWRE.nds`. `--write` writes the C files, the `delinks.txt` and `symbols.txt` lines and the `data_inc.txt` rows, formats the files, runs `make` and `make check`, then `make decomp-dev-report`, and stops naming any written file whose data the report scores below 100%. The same inputs give the same output, and a second run finds nothing left to write. A wrong result is fixed in the tool or the config it reads, not by hand in the output.

## What it does

- Objects: one starts at each 4-aligned dsd symbol in a `.data` or `.rodata` gap that is not inside the declared size of the one before (`bss.build_items`). Every other symbol is an alias (below).
- Size: the declared size; else, for a string (3 or more printable characters, a zero, then fewer than 4 zero bytes up to the next symbol), its length with the zero; else the size in `symbols.txt`; else the bytes up to the next symbol. The bytes from the end of the object to the next multiple of 4 must be zero: they are the linker's `ALIGNALL(4)` pad. A declared size whose pad is not zero is too small, and the object takes the bytes up to the next symbol instead. `symbols.txt` gets each object's size as `kind:data(byte[N])`, so objdiff compares the same size on both sides, and each alias `kind:data(byte[0])`.
- Filler: bytes no symbol covers become placeholders named by address (`data_ov004_022bf13c`), cut to keep the runs few (`bss.split_filler`). Fewer than 32 zero bytes before a 32-aligned object become `__attribute__((aligned(32)))` on it.
- Type: the object's declaration (`bss.chosen_decl`: the host's own, else a header's, else the most common one whose type a header defines). A struct's layout comes from `build/dsindex.json` and becomes a `data_inc` struct row, with `pad[N]` for the gaps; bit fields, unions and floats stop it. Without a usable declaration: `char` for a string, `void*` for pointers, `u32` when the size is a multiple of 4, `u16` of 2, else `u8`. A relocation must sit on a 4-byte field. A declaration the tool cannot write stops a host; in a new file the object is written from its bytes, with a comment naming the declaration.
- Values: the initializer includes `data/<module>/<symbol>.inc.c`, written by `data_inc.py` from a row `--write` adds to `config/AWRE/data_inc.txt`. An object whose words are all pointers or zero is written out instead (`{&a, &b, 0}`): names, not game bytes.
- `const`: an object in `.rodata` is defined `const` (a pointer table as `T* const`), one in `.data` never is, since mwcc sends `const` objects to `.rodata`.
- Zero: mwcc puts an object whose initializer is all zero in `.bss`. A file's definitions that include one go between `#pragma explicit_zero_data on` and `reset`.
- Order and files: mwcc sorts a file's initialised objects by size, starting from the reverse of the definition order (`tools/mwcc_order.py`), so a file holds a contiguous slice of one ascending run, and a run may span several files: the `.bss` rule (`docs/bss-tool.md`, decision a). A slice goes into the `complete` file that uses its objects when that file has no data of its own yet (`.bss` is fine if its order does not change) and compiles with the definitions in place; else into a new file in the users' directory, named `<directory>_data.c` or `<directory>_rodata.c` in a topic directory and `ovNNN_data_<address>.c` at the module level. A file takes one slice in all its data sections.
- Names: the definition replaces the host's `extern` statements. Names the initializers point at are declared from a header when one declares them, else as `extern u8 NAME[];` or `void NAME(void);`.
- Checks before writing: each file is compiled; its objects must come out in the slice's section, in address order, with the ROM's sizes, and a host's `.bss` must keep its order.

## Aliases

An alias is a symbol inside an object: a field or element address that code loads from a literal pool, or a separate object the one before swallows. The linker script defines it (`bss.py lcf` now writes the aliases of `.data` and `.rodata` ranges too), and `config/AWRE/aliases.txt` must list it with its group, as for `.bss` (`docs/bss-tool.md`, "Aliases"; issue 0083). `plan` stops on an alias the list does not have, and on one that lies past the declared size of its object; `scan` prints both and goes on. An alias at the very end of a declared array, at an address that is not 4-aligned, usually means the two names are rows of one array: declare the object with the size that covers both.

## Relocations to check

dsd takes some words for pointers that are not: two 16-bit values whose bytes happen to look like an address (the ov003 pilot below). `data.py relocs MODULE` lists the relocations in the module's data that may be wrong: one that is not on a 4-byte boundary, one with an odd target, one where the object's declared type has no 4-byte field, and one into `.bss` or another module whose target symbol nothing else refers to. Each comes with the functions that read the object. Check each against that code (`python3 tools/fn.py show`, `python3 tools/ds.py field`), and delete it from `relocs.txt` only when the code proves it is not a pointer; delete the target symbol too when nothing else refers to it.

Found so far:

- main `.rodata` and `.data`, ov002 `.data`: of 8,618 relocations, 43 were false and are gone: 11 inside LZ77-compressed streams, 12 in the cell offset tables of sprite files, 12 in the sine and cosine table, 7 in tables of halfwords or bytes, and 1 in a `BattleSideWeapon` record. No missed pointers turned up. `data.py relocs` does not flag these kinds yet; it should flag a relocation inside an LZ77 block, a sprite file or the sine table.

## What it cannot do yet

- Nested arrays of structs in a struct, unions, bit fields and floats: the object needs a declaration the tool can write, or it is written from its bytes in a new file.
- A host that already has `.data` or `.rodata` of its own takes no slice, so its neighbours go to new files.
- An object whose `extern` declarations disagree across files keeps the chosen one; the others stay as they are.
- Array sizes given by a macro (`s16 x[2][CELL_PAIR_COUNT]`): the tool does not expand them and writes the object from its bytes in a new file. Write the size as a number in the declaration (`include/game/ov006_cell_forms.h`).
- String literals: a string the original wrote as a literal (with a compiler-made name) is written as a named array. Whether that differs from the original object is not known.

## What the ov003 pilot found

The pilot looked at ov003's `.data`: 0x02364880-0x02365b00, 4,736 bytes, 122 dsd symbols, 120 relocations.

- Runs: with each symbol's size taken as the distance to the next symbol, ov003's `.data` falls into 15 runs of ascending size, from 2 to 51 symbols each. The first run (`Ov003_StartMessage` 8, `Ov003_gMainLayerConfig` 80, `Ov003_gSubLayerConfig` 80) is used by two files, `ov003_update_link_start.c` and `ov003_init_screens.c`, in the same order as their `.text`. So a run holds the `.data` of one or more original files, as with `.bss`.
- Size: the distance to the next symbol includes the linker's pad to 4. A string of 23 bytes shows as 24; `char x[24]` gives one byte more than the original object, and objdiff compares symbol sizes.
- Structs with pointers (`GfxLayer`): `data_inc.py` takes the field types in braces, `{u8,u8,u8[2],u32,u32,u16*,u8,u8[3]}`, with pointer fields as `(u16*)&data_02177b08`.
- Score: `Ov003_StartMessage` (string) and the two layer configs (structs with relocations) link at their ROM addresses, and the report scores both units' `.data` at 100%.
- Relocations: 12 that dsd added were wrong, and were deleted. Eleven sit in the quad frames listed by the table at 0x02365960, which `AnimSpriteTask_Draw` reads as 8-byte `QuadSpritePart` records whose second word is a u16 tile and two bytes of size and flip bits; dsd read that word as a main `.bss` address (`a0 00 22 02` as `0x022200a0`). The twelfth, at `data_ov003_023651f0` +0x24, falls in 6-byte records that `Ov003_LoadSpawns` reads one byte at a time. The 12 main `.bss` symbols at the targets were false as well and were deleted from `symbols.txt`.
- A pointer object needs no `data_inc.txt` row: `data_ov003_02364928` holds `&data_ov003_02365b14` and is written that way.
