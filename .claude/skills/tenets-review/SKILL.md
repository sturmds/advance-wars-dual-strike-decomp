---
name: tenets-review
description: Review pass for every generated change in this repo before it is offered to sturmds. Checks C against TENETS.md and checks prose (comments, docs, commit messages) against docs/style. Run it after a function matches and before committing.
---

# Tenets review

Run this on the change about to be committed: the staged diff plus the planned commit message. Fix what you find, then rerun until it is clean. Report what you changed in one short list.

## 1. Mechanical checks

```sh
git diff --cached --name-only
python3 tools/style_check.py <changed .c .h .md .py files> <file holding the commit message>
```

- Any `error:` must be fixed. These words are in `docs/style/banned.txt`.
- Each `warning: claudeism` needs a judgment call. Keep the word if it is the plain technical term (`diff`, `sha`, `pinned` for a tool version). Replace it if a plainer word says the same thing.
- Each `warning: possible 'X is the Y'` needs reading. The aphorism form is banned: "the hash is the test", "the linker script is the map". Rewrite those to say what the thing does ("`make release` fails if any byte differs"). Plain statements of fact that happen to use "is the" ("ARM7 is the second CPU") can stay.

## 2. C against the tenets

For each changed function:

1. **Match.** It still matches (`make release` passes). No change may cost a match.
2. **Readable once the meaning is known.** If the function's purpose is clear, it has a behavior name, not `func_XXXXXXXX`. Locals and parameters have real names, not `r0`, `v1`, `self` as an `int`.
3. **Types.** Pointers to known structs use the struct type. No `int self` with offset arithmetic. No `*(s16 *)(p + 0x2c)` when a struct with that field exists or the field's meaning is known. If the struct is only partly known, add the known fields to the shared header in `include/` with `unk_XX` padding, instead of a private per-file copy.
4. **Casts.** Remove casts the compiler does not need for the match. Keep ones that change codegen, with a short comment.
5. **Constants.** Known values get named constants or enums (unit types, terrain, button masks).
6. **Comments.** Only explain why the code has an odd shape ("needed to match: ..."), or game facts that are not obvious from the code. No comments that restate the next line.
7. **Declarations (tenet 11).** No copy of a declaration that a header has: `python3 tools/decls.py check` must pass. A new function or global goes in the header of its topic, not in the caller. A local declaration that has to differ from the header is listed in `config/AWRE/local_decls.txt` with why. A new macro that other files define the same way goes in the struct's header.
8. **Hard-coded addresses.** Flag any literal that looks like a pointer (`0x02xxxxxx`, `0x027xxxxx`, `0x04xxxxxx` outside register defines). Matching builds tolerate them; modding (plan M3) does not. Prefer a symbol; if it has to stay for now, list it in the report.

## 3. Prose

Comments, docs and commit messages: plain, short, no hype, no filler. Write it the way you'd say it to a teammate. Commit messages: an imperative subject line under 72 characters, then what changed and why.

## 4. Record the review

When the change is clean, write down what you checked and found, one or more lines per file the change adds or edits under `src/` or `include/`, in a findings file outside the repo (the git directory works: `F=$(git rev-parse --git-path review-findings.txt)`):

```
src/game/main/unit/unit_get_hp.c: named Unit_GetHp; Unit * param; no casts or addresses; no comments needed
include/game/unit.h: added hp at 0x2c (read by Unit_GetHp); kept unk_ padding around it
```

Each line is `<path>: <findings>` with at least 20 characters of findings. Say what you fixed and what you left (a kept cast and why, a hard-coded address). Then stage the change as it will be committed and record the review:

```sh
python3 tools/review_stamp.py --findings "$F"              # the staged change (or HEAD if nothing is staged)
python3 tools/review_stamp.py --findings "$F" <hash>...    # commits you reviewed after committing them
```

`review_stamp.py` refuses the stamp when a changed file has no findings line, or when `tools/style_check.py` finds a banned word in a changed file. The stamp holds the findings and is kept under the git directory, outside the repo, keyed by the diff's patch ID. `land_self.py` prints the findings into its log. Any later edit to the change needs another review and stamp, and so does a commit you fix by hand during `rebase_onto_mainline.py --continue`; a commit the replay merges without stopping keeps its stamp. `land_self.py` refuses a branch whose commits touch `src/` or `include/` without a stamp (exit 5); the exemptions (`Propagate ...` commits that only add files, `WIP ...` commits that leave `include/` alone) are listed in `tools/review_stamp.py`. Stamp only what you reviewed.

## Output

A list of what you changed and anything left for sturmds to decide (unnamed functions whose meaning is unclear, hard-coded addresses kept, claudeism warnings you kept and why). Do not offer the change until `make release` passes after your edits.
