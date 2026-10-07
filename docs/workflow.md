# Decompiling one function

Everything runs locally. The examples use `PackNibblePairs` (main, 0x020803fc), one of the three M0 proof functions.

## 1. Pick and look

```sh
make                              # builds once, so build/delinks/ exists
python3 tools/fn.py show 0x020803fc
```

`show` prints the module, ARM or Thumb, the size, the source file that covers the function (if any), and the original disassembly with relocations.

Good first picks: leaf functions (no `bl`), and callers of functions that already match.

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

## 5. Mark it complete and review

1. `python3 tools/fn.py finish NAME` adds `complete` under the file entry in `delinks.txt`. The link now uses your object instead of the original bytes.
2. Run the review pass (`.claude/skills/tenets-review/`): names, types, no magic offsets, comments only where the shape is odd, plain writing.
3. `make release`. It fails if any module or the whole ROM differs from retail.
4. Commit. The hooks run `make release` again and check the message.

## Reference

| Command | What it does |
|---|---|
| `python3 tools/fn.py show NAME\|ADDR` | Location and original disassembly |
| `python3 tools/fn.py diff NAME` | Compile, diff, exit 0 on a full match |
| `make` | Build `build/AWRE.nds` |
| `make check` | dsd module check, per-module SHA-1, whole-ROM SHA-1 |
| `make progress` | Matched functions and bytes: game vs SDK, C vs assembly |
| `ninja objdiff.json` | objdiff project file, for the objdiff GUI if you install it |
