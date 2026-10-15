---
name: organize-directory
description: Make one source directory of advance-wars-dual-strike-decomp readable under tenet 11 - one declaration per name in the header of its topic, shared macros in headers, neighbouring functions of one topic merged into a file with its header - without changing any function's code. Takes the directory (for example `menus` or `src/game/main/menus`). Follows docs/readability.md; issue 0089 tracks the work.
---

# Organize a directory

You get a directory, for example `menus`, meaning `src/game/main/menus/`. Goal: every function defined there is declared once, in a header a reader would guess. Neighbouring functions of one topic share a source file with its own header. No C file copies a declaration that a header has. The ROM still matches and no function's code changes.

Read first: `docs/readability.md` (the rules and the tools), tenet 11 in `TENETS.md`, issue `docs/issues/0089-function-files-and-headers.md` (what was done and learned), and "Merging files" in `docs/workflow.md`. `src/game/main/ai/` and `include/game/ai/` show the finished result.

## Rules

- A readability step leaves every function's code as it was. Never change code to make something fit.
- Change a declaration's place or its type, never both in one step.
- No broad `sed` over C files. Use the tools, or edit the lines you mean: a loose pattern once deleted a line inside a call.
- No renames of functions, fields or globals here; that is other work.
- One directory at a time, with no loop branches open: these changes touch callers all over `src/`.
- Do not commit until sturmds says so.

## Setup

1. Work on a new branch from mainline, clean tree.
2. `make && make check`, then `python3 tools/objcmp.py snap`. Every later check compares against this snapshot of all functions' code.

After every step:

```sh
make && make check && python3 tools/objcmp.py compare
git add -A          # checkpoint; `git checkout -- src include config` undoes a bad step
```

If `make` fails, the `make check` line is from the old build. After `decls.py unify` or `reconcile`, run `make` again before `compare`.

## Steps

1. **Survey.** From `config/AWRE/arm9/delinks.txt`, list the `.text` ranges of the directory's files in address order, and note the files of other directories between them. Propose topic groups: each one contiguous range of this directory's files, named by what the code does. Show the groups to sturmds before merging.
2. **Homes for names.** `python3 tools/decls.py check --list` and the copies of each name. Give each widely copied global or callee its home with `decls.py declare HEADER NAME...`: a variable goes in the header that defines its type; a function in its topic header, else in `include/game/<dir>.h`. Do not leave a name in whatever header declared it first.
3. **Macros.** `grep -h '^#define' src/game/main/<dir>/*.c | sort | uniq -c | sort -rn`.
   - One meaning under several names: `decls.py macro HEADER NEW OLD...`, in the header of the struct the macro reads. Look for an existing macro with that meaning first (`MAP_STATE`, `MAP_TILES`, `MAP_TILE_INDEX`, `MAP_TILE_AT` in `game/Unk_02080440.h`); the tool refuses a NEW name that is already a macro with another body.
   - One name with several meanings (`NO_TILE` was 9999 in some files and 0x7fff in others): give each meaning its own name by hand.
   - Leave forms written for a match (`rowStart[y] + x` changed 10 functions; pointer sums with `sizeof`).
4. **Merge.** For each group, `python3 tools/merge_files.py --dry-run src/game/main/<dir>/TOPIC.c FILES...`, then without `--dry-run`.
   - A file with a `#pragma` is not merged: mwccarm applies it to the whole file. It keeps its own file.
   - "declared differently": run `decls.py unify NAME --apply`, with `--variant` for each width in use. Callees that change together (several taking one `u8` unit type) are tried together.
   - When no one prototype works for every caller, the functions came from different original files: `merge_files.py --split FILE.c FUNC [--rest REST.c]`, keep the moved function's own declaration with a "needed to match" comment, and list it in `config/AWRE/local_decls.txt`.
5. **Changed functions.** When `compare` lists any, compare the callees' declarations in the function's old file (`git show HEAD:path`) with what it sees now. Try `unify` first. Then the caller's local types: a narrower local (`s16` where an `int` gets narrowed) is often what the original had. Use a cast only when neither works, with a comment.
6. **The rest.** Functions of the directory in no topic file: `decls.py declare game/<dir>.h --dir src/game/main/<dir>`.
7. **Clean up.** `decls.py dedupe`, `decls.py reconcile --write`, `dedupe` again. Try `unify` on each name `reconcile` kept. Remove the `local_decls.txt` lines that `check` reports as stale; give each kept line a reason that says why.

## Finish

- `python3 tools/decls.py check` reports 0, and `make release` passes.
- Add a progress line to issue 0089: files before and after, headers added, prototypes `unify` changed, lines added to `local_decls.txt`, and anything new about the compiler.
- Run the tenets review (`.claude/skills/tenets-review/`), then report to sturmds: the groups, what changed outside the directory, the kept declarations, and anything you left. Stop there.
