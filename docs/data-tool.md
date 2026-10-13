# Linking .data from C: ov003 pilot

Step 3a of issue 0082: what it takes to define initialised `.data` in C so that it links from `complete` files, as `.bss` does now (`docs/bss-tool.md`). The pilot looked at ov003's `.data`: 0x02364880-0x02365b00, 4,736 bytes, 122 dsd symbols, 120 relocations. Game bytes stay out of git: every value comes from the builder's ROM through `tools/data_inc.py` (`docs/workflow.md`, "Data from the ROM").

## What the pilot found

### 1. Runs of ascending size

Yes. With each symbol's size taken as the distance to the next symbol, ov003's `.data` falls into 15 runs of ascending size, from 2 to 51 symbols each. The first run (`Ov003_StartMessage` 8, `Ov003_gMainLayerConfig` 80, `Ov003_gSubLayerConfig` 80) is used by two files, `ov003_update_link_start.c` and `ov003_init_screens.c`, in the same order as their `.text`. So a run holds the `.data` of one or more original files, as with `.bss`, and the `.data` can go into the code file that uses it.

The distance to the next symbol includes the linker's pad to 4. A string of 23 bytes shows as 24. The real size matters for strings: `char x[24]` gives one byte more than the original object, and objdiff compares symbol sizes.

### 2. What data_inc.py can express

- Strings and byte tables: yes, as `u8` (`char Ov003_StartMessage[8] = { #include ... }`).
- Words and pointer tables: yes, as `u32` or `void*`; a relocated word comes out as `&symbol`.
- Structs with pointers (`GfxLayer`): not before this pilot. A `u32` array has the wrong type for the users' `extern GfxLayer[4]`. The pilot adds a struct element type: field types in braces, `{u8,u8,u8[2],u32,u32,u16*,u8,u8[3]}`, which writes one `{...}` per struct, with pointer fields as `(u16*)&data_02177b08`.
- Still missing: nested structs, bit fields, and a `data_inc.txt` row per object that takes its size from `symbols.txt` (each row now repeats address and size by hand). The struct layout is written twice, in the header and in `data_inc.txt`; `tools/ds.py` could supply it from the compiler.

### 3. Address and objdiff score

Both tried objects match. `Ov003_StartMessage` (string, in `ov003_update_link_start.c`) and the two layer configs (structs with relocations, in `ov003_init_screens.c`) link at their ROM addresses; `make check` passes, and `make decomp-dev-report` scores both units' `.data` at 100% (8 and 160 bytes). The two 80-byte objects came out in source order. Each file needed only a `.data` line in its `delinks.txt` entry.

### 4. Relocations

- No missed pointers: `tools/scan_pointers.py` lists 18 candidates in ov003's `.data`, none high. The two medium ones (`0x02020074`, `0x0202009c`) and the low ones are pairs of 16-bit values.
- At least 12 relocations that dsd added are wrong. They are halfword pairs that happen to read as main `.bss` addresses: `data_ov003_023657d8` +0x14, `data_ov003_02365800` to `023658c0` +0x1c (to `data_022200a0`...`022200e0`), `data_ov003_023659d4` (2) and `data_ov003_02365a64` (3, to `data_02220060`, `data_02200070` and similar). `data_ov003_023651f0` +0x24 (to `data_02190903`, an odd address) is likely one more. The tables are records of 16-bit fields (`d441 df41 00f0 0022 ...`). These relocations must be deleted from `relocs.txt` before the tables are written as plain values, or objdiff scores them 0%. The main `.bss` symbols dsd made for these targets may be false too.

### 5. Requirements for a data.py

1. Read `.data` and `.rodata` ranges from `delinks.txt`, symbols from `symbols.txt`, relocations from `relocs.txt`, users from `build/fnindex.json`, and declared types from `build/dsindex.json`.
2. Split into ascending runs, then cut each run where the user file changes, as `bss.py` does. Put a slice into its users' file when that file's `.text` order agrees; otherwise into a new file next to them.
3. Find each object's real size: for a string, the length up to the first zero plus one; otherwise the declared type. Write it into `symbols.txt` (`kind:data(byte[23])`) when the pad would make it look bigger.
4. Choose the element type: `u8` for strings and bytes, the struct layout from the declared type, `void*` for pointer tables, `u32` otherwise. Stop on a relocation in the middle of a field that is not 4 bytes.
5. Flag relocations whose target is not 4-aligned or sits in a table whose other words look like halfword pairs, for a person to check before deleting them.
6. Write the `data_inc.txt` rows, the C definitions in definition order (`tools/mwcc_order.py`, `definition_order`), and the `.data` lines in `delinks.txt`; then run `make`, `make check` and the objdiff report, and name any unit below 100%.
7. Handle `const` objects: mwcc sends them to `.rodata`, so `.rodata` needs `const` and `.data` must not have it.

## Open questions

- Some ov003 strings (`cmfade`, `COMBATSR!FINHDEL`) may be string literals in the original rather than named arrays. A literal gets a compiler-made name; whether objdiff still matches a named array against it is untested.
- ov003's `.data` starts at a 32-byte boundary after `.ctor`. The first object needed nothing special; the alignment of objects further in was not checked.
