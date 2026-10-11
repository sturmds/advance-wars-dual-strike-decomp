# Decompiling one function

Everything runs locally. The examples use `PackNibblePairs` (main, 0x020803fc), one of the three M0 proof functions.

## 1. Pick and look

```sh
make                              # builds once, so build/delinks/ exists
python3 tools/fn.py show 0x020803fc
```

`show` prints the module, ARM or Thumb, the size, the source file that covers the function (if any), its callers and other references (`refs:`: pointer tables such as `data_021585c0+0x14c`, and functions that load its address as a callback), and the original disassembly with relocations.

Good first picks: leaf functions (no `bl`), and callers of functions that already match.

### Structs and data

```sh
python3 tools/ds.py show Task                 # layout of a struct in include/
python3 tools/ds.py field Task.unk_64          # functions that use a field, and how the original loads it
python3 tools/ds.py show data_02153634         # a data symbol: type, references, bytes
```

`ds.py` does for data what `show` does for functions (issue 0041).

- `show STRUCT` prints each field's offset, size, type and name, bit fields as `:lsb+width`, gaps between fields, the header, the data symbols declared with the type, and how many C files use each field (`--uses` lists the functions).
- `show data_ADDR|gName|0xADDR` prints the module, kind, size to the next symbol, declared type, the functions and tables that reference it, and its bytes decoded with the declared type (`--count N` table entries). If the symbol lies inside another symbol's declared size, it says so (`inside: gCoTable+0x28`). A symbol with a declared size lists the later symbols it covers.
- `dupes` lists likely duplicates for renames jobs: a type defined twice under one name, header structs with the same size and field offsets, types in C files whose fields line up with a header struct, union views that lie within another, symbols declared with two types, and `#define`s repeated in C files or matching a header constant's value. They are candidates: small structs share layouts by chance. `--kind K` shows one list, `--all` every row.
- `field STRUCT.FIELD` lists each function in `src/` that names the field. For each, it shows the original instructions at the field's offset (`ldrsh`, `ldrh`, `strh`, ...) and any `(s16 *)&` casts in our C. A closing `check:` line says when the declared signedness disagrees with every load. Fix the type in the header rather than adding a cast.

Layouts come from the compiler: `tools/dsindex.py` compiles a small probe for each header with `-g` and reads the DWARF debug data, so unions, bit fields and padding are exact. C files that define their own types are probed the same way with their function bodies cut out. It writes `build/dsindex.json` in about three seconds and then reprobes only the files that changed; `make dsindex` runs it. Field uses are found by name, in the C files that include the struct's header, so two structs with a field of the same name in one file both show up. Loads are read only for functions that already have C, and only at the field's offset in that struct.

## 2. Give it a file

Add an entry to the module's `delinks.txt` (main: `config/AWRE/arm9/delinks.txt`; overlays: `config/AWRE/arm9/overlays/ovNNN/delinks.txt`). Leave out `complete` for now:

```
src/game/pack_nibbles.c:
    .text       start:0x020803fc end:0x02080440
```

`end` is the next function's address, so alignment padding stays inside the file. A file can hold several neighbouring functions; start with one, and merge files as evidence appears (shared static data, functions that always sit together).

Rename the function in `symbols.txt` once you know what it does (TENETS 4). Rename data symbols it uses the same way. C code refers to symbols by these names.

## 3. Write C

Put the file at the path from step 2. Shared types go in `include/`; SDK declarations in `include/sdk/`.

- Compiler: mwccarm 2.0-72, `-O4,p -proc arm946e -interworking`. `-ipa` does not exist in this version, so each function compiles on its own.
- Thumb SDK assembly: write it as `asm void` under `#pragma thumb on` (see `src/sdk/fx/fx_mtx43.c`). The game code itself has no Thumb.
- Shape matters more than meaning at first: declaration order, `for` vs `while`, a one-case `switch` instead of an `if`, temporaries. The compiler is old and literal.

## 4. Diff until it matches

```sh
python3 tools/fn.py diff PackNibblePairs
```

This compiles the file, rebuilds the original's delinked object, and runs `objdiff-cli`. Lines marked `|` differ; relocations are compared by symbol name. The exit code is 0 only at 100%.

### Fast check

```sh
python3 tools/fn.py check src/game/pack_nibbles.c PackNibblePairs
```

`check` compiles one file with the build's compiler command into a temporary object and compares one function with `build/fnindex.json`. It needs no `delinks.txt` entry, and runs neither ninja nor dsd: about 3 ms per function in-process for a small file and 100 ms for the largest, plus 0.1 s of Python start-up from the command line. Exit 0 on a match; otherwise it prints how many instructions differ, position by position (an inserted instruction makes everything after it differ, where `diff` lines them up).

It compares the bytes outside relocations, the relocation offsets and kinds, and the relocation targets as addresses, so `Table+0x1c` equals dsd's symbol at that address (issue 0017). A relocation to a symbol it cannot place (a string or `static` data the object defines, missing from `symbols.txt`) counts as a difference. `tools/tests/test_fncheck.py` checks that it never says yes where `diff` says no. Use it to try many variants, then run `diff` and `make release` on the one that matches.

`build/fnindex.json` lists every function: module, mode, address, size, original bytes, relocations, a masked hash (equal for functions that differ only in what they call or load), the mnemonics before the literal pool, and whether a C file covers it. `make fnindex` builds it in about a second; `check` rebuilds it when `symbols.txt`, `relocs.txt` or `extract/` change. It does not watch `delinks.txt`, so `status` can lag behind `fn.py add` until the next `make fnindex`. Tools can `import fnindex` and call `fnindex.load()` once, then `fnindex.check(index, path, name)` per variant.

## Tools that match many functions

All three work from `build/fnindex.json`, check every candidate with the fast check, and `fn.py diff` each before `fn.py finish`. They write `src/game/<module>/func_XXXXXXXX.c`, `delinks.txt` entries and nothing else. Run them on `mainline` with a clean tree, build and commit the result in chunks (each commit must pass `make release`). All are based on khdays-decomp (`docs/third-party/khdays-decomp.md`).

- `python3 tools/propagate.py [--dry-run] [--identical-only]`: copies the C of a matched function to unmatched functions with the same masked hash (identical twins) or the same shape with other immediates (near twins). `fn.py pick --families` ranks the unmatched families; match a head by hand, then run this.
- `python3 tools/trivial.py [--pattern P] [--dry-run]`: writes C for functions of up to nine instructions: empty, constants, getters, stores, load-modify-store, tail calls, wrappers, comparisons. Patterns: empty, const, getter, tailcall, wrapper, store, modify, addr, compare, other.
- `python3 tools/permute.py FILE.c NAME [--budget-seconds 120] [--apply]`: for a started function below 100%, tries declaration orders, statement swaps and local types, and prints the best variants. `--apply` replaces the source when the best one matches.

What they leave behind is in `docs/issues/0024-brute-force-tool-leftovers.md`.

## 5. Mark it complete and review

1. `python3 tools/fn.py finish NAME` adds `complete` under the file entry in `delinks.txt`. The link now uses your object instead of the original bytes.
2. Run the review pass (`.claude/skills/tenets-review/`): names, types, no magic offsets, comments only where the shape is odd, plain writing.
3. `make release`. It fails if any module or the whole ROM differs from retail.
4. Commit. The hooks run `make release` again, rewrite `PROGRESS.md` and stage it, and check the message.

## Reference

| Command | What it does |
|---|---|
| `python3 tools/fn.py show NAME\|ADDR` | Location, callers, table references and original disassembly |
| `python3 tools/fn.py diff NAME` | Compile, diff, exit 0 on a full match |
| `python3 tools/fn.py check FILE.c NAME` | Compile one file, compare one function with the index, exit 0 on a match |
| `python3 tools/fn.py pick --families` | Rank unmatched same-shape families by member count |
| `make fnindex` | Build `build/fnindex.json` |
| `python3 tools/ds.py show STRUCT\|data_ADDR` | Struct layout, or a data symbol's type, references and decoded bytes |
| `python3 tools/ds.py field STRUCT.FIELD` | Functions that use a field, the original loads and stores, signedness check |
| `python3 tools/ds.py dupes [--kind K]` | Likely duplicate types, extern clashes and repeated constants |
| `make dsindex` | Build `build/dsindex.json` |
| `python3 tools/tests/test_fncheck.py` | Check that `check` agrees with `diff` |
| `make` | Build `build/AWRE.nds` |
| `make check` | dsd module check, per-module SHA-1, whole-ROM SHA-1 |
| `make progress` | Matched functions and bytes: game vs SDK, C vs assembly |
| `python3 tools/progress.py --write PROGRESS.md` | Same counts per module, plus C files not linked yet; the pre-commit hook runs it |
| `ninja objdiff.json` | objdiff project file, for the objdiff GUI if you install it |
