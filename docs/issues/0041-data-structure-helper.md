# 0041: No tool shows data structures the way `fn.py show` shows functions

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

`fn.py show` tells an agent where a function is, who calls it and what points at it. Nothing does the same for data: structs in `include/game/` (270 headers, 64 of them `Unk_*`) and about 2,500 data symbols in `config/AWRE/arm9/symbols.txt`. Agents work out layouts, field uses and signedness by hand, and the same facts get rediscovered and fixed late:

- Signedness. Fields declared unsigned that the original reads with `ldrsh` or `ldrsb`, so agents add `*(s16 *)&` casts to match. `Task.unk_64` was cast this way by three agents before renames16 made it `s16` and removed the casts from 28 files. The same happened with `PageStripTask.mapId`, `MainMenuTask.unk_4c`, `PlayerInfoSpriteTask.unk_2e`/`unk_30`, `SpriteEntry.x`/`y` and others.
- Duplicate views. Local struct copies, extra `MenuState` union members and `Unk_*` structs that describe the same memory (issue 0014). Every renames job spends time folding them into the shared headers.
- Wrong field comments. Comments that say a field is a script when it is a file name (`MapHeader.unk_0c`), a turn count when it is a path cost (`MoveSearchNode.turn`), and so on. Nobody can see all the readers of a field at once to check.
- Data symbols that are only offsets into another table (`data_02153634`, `data_021536dc`, `data_022a72d8` into `gCoTable`), found by chance.

## Notes

Scope to decide before building. A starting proposal, as a new `tools/ds.py` beside `fn.py`:

- `ds.py show STRUCT|data_ADDR|gName`
  - For a struct: each field's offset, size, type and name, with `unk_`/`pad_` counts; the header that defines it; the data symbols declared with that type; the functions in `src/` that read or write each field.
  - For a data symbol: address, size, section, the type it is declared with, the functions and tables that reference it (from `relocs.txt`, as `fn.py show` does for `refs:`), and its bytes from `extract/` decoded with that type when there is one.
- `ds.py field Struct.field`: readers and writers by function, and how the original code loads it (`ldrh` or `ldrsh`, `ldrb` or `ldrsb`, word), so a type that disagrees with every load stands out.
- `ds.py dupes`: structs, local copies and externs that cover the same address or have the same layout; overlapping union members. Input for issue 0014 and for renames jobs.

Open questions:

1. Offsets. Parsing the headers in Python is fragile with unions, bitfields and padding. Compiling a generated probe file with mwccarm that records `sizeof` and `offsetof` for every struct gives exact numbers from the toolchain we already pin. Which one, and how is the probe output read back (object file data, or a symbol table)?
2. Field uses. Finding `->field` and `.field` in our C is easy but misses casts and macros. Mapping loads in the original code to fields needs each function's base pointer type, which only works for functions already in C (from their objects and relocations). How far to go in the first version?
3. Speed and caching. `fn.py show` reads `build/fnindex.json`; a struct index could be written the same way by `make`.
4. Where agents use it. A line in the decomp-function skill and `jobs/common.md`, like the `refs:` rule from issue 0003.

Constraints: read-only, no change to `fn.py` output or to landing tools, a test in `tools/tests/` that needs no ROM where possible, `make test` and `make release` pass. It can land while the loop runs.

Suggested order: `show` and `field` first. `dupes` after batch 21, when renames can act on it without colliding with decompiling agents.

## Done when

The scope above is decided and written down here, and `ds.py show` and `ds.py field` exist and are listed in `docs/workflow.md`. `dupes` may be a follow-up issue.
