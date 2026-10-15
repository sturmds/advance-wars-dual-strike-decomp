---
name: decomp-function
description: Decompile one AWRE function to matching C inside a git worktree of advance-wars-dual-strike-decomp, then commit it on the worktree's branch. Used by agents the orchestrator starts (docs/loop.md); also works by hand in the main checkout.
---

# Decompile one function

You get a function name (for example `func_02080440`) and work in your own git worktree of `advance-wars-dual-strike-decomp`. Goal: C that compiles to the same bytes, readable under `TENETS.md`, committed on your branch. Read `CLAUDE.md` and `TENETS.md` first if you have not.

## Rules

- No network, no installs, no sudo, no push, no `--no-verify`.
- Touch only: your C file, headers you need under `include/`, `config/AWRE/local_decls.txt`, and config through `tools/fn.py` (add, complete, rename). Do not edit `tools/`, `Makefile`, `hooks/` or `docs/`.
- Do not rename symbols other agents may be working on unless your function makes their meaning clear (callees and data you use are fine).
- Writing: plain, short. No words from `docs/style/banned.txt`. Never the "X is the Y" aphorism.
- Paths: the shell may reset its directory between calls, so use absolute paths (worktree root + path) in every command.
- Commands: run one plain command per Bash call (no `&&` chains, no heredocs); the worktree sandbox refuses compound commands. Read files with the Read tool, not `cat`.
- Never write to a shared scratch directory: other agents run at the same time. Pass commit messages inline with `git commit -m ... -m ...`; if the sandbox refuses a long `-m`, write the message to a file in your own scratch subfolder and use `git commit -F` (issue 0080).
- Waiting for a process: wait on its PID (`tail --pid=<pid> -f /dev/null`) or on its lock (`flock <lock file> true`). Never a `pgrep -f` loop: the pattern matches the loop's own command line, so it never ends and stays behind after you stop (issue 0073). When you stop a run of `permuter.py` or anything with worker processes, kill its workers too and check with `ps -p` that none are left.
- Run `tools/land_self.py` on its own, never piped into `head` or another command that can end it early: an interrupted landing leaves a replay to abort (issue 0075).
- Results file: if the Write tool refuses its path in the main checkout, write it in your scratch subfolder and `cp` it there. Its first line is exactly `RESULT: <matched|partial|failed>` (or `done` for jobs).

## 0. Setup (worktrees only)

```sh
sh tools/wt_setup.sh     # links orig/ and .cache/ from the main checkout
make                     # first build, ~5 s
```

## 1. Look

```sh
python3 tools/fn.py show NAME     # needs one `make` first
```

Read the callers, callees, data (with the module, kind and other functions that use each symbol) and disassembly. `show` on a caller or callee helps when the meaning is unclear. Guessed names stay guesses: say so in NOTES.

For the data it touches, `python3 tools/ds.py show STRUCT` prints a struct's layout from the compiler and `ds.py show data_XXXXXXXX` a data symbol's declared type, references, containing table and bytes. Before you write a cast such as `*(s16 *)&p->field`, run `ds.py field Struct.field`: if its `check:` line says the declared signedness disagrees with every load, put the type fix in NOTES instead (see `docs/workflow.md`, "Structs and data").

## 2. Give it a file

```sh
python3 tools/fn.py add NAME src/game/<file>.c
```

File placement (see "Source layout" in `CLAUDE.md`): `<module>` is `main`, `itcm` or `ovNNN`.

- Look at the directories under `src/game/<module>/` first. If your function belongs to one (`copowers/`, `skills/`, ...), put it there: `src/game/main/copowers/tsunami.c`.
- If it belongs to a clear topic with no directory yet, and you can show the topic from the code, create the directory and name it in your commit message and NOTES.
- If you cannot tell what it does, use `src/game/<module>/func_XXXXXXXX.c`. Rename moves the file within its directory; move it into a topic directory with `git mv` and fix its path in `delinks.txt` once the meaning is clear.
- File names are snake_case. One function per file for now.

## 3. Write C

- Start from `#include "types.h"` (u8, s16, u32, fx32, BOOL...).
- **Declarations come from headers** (`docs/readability.md`): before declaring a callee or a global, `grep -rn 'NAME' include/` and include the header that declares it; never copy its declaration into your file, `make release` fails on a copy. A callee or global that no header declares: add its declaration to the header of its topic (the header of the struct a global has, the header of the directory or file the callee is defined in), or, for a callee not yet decompiled, declare it at the top of your file with its current name and your best-guess signature: `void func_0208c21c(int a, int b);`. If your function only matches with another type than the header's, keep your own declaration and add a `path name needed to match: ...` line to `config/AWRE/local_decls.txt`.
- **Data:** refer to globals by their `symbols.txt` names, through their header as above, or as `extern` when no header declares them: `extern u32 data_0229d8c4;`. Never put string literals, `static` data or `const` tables in the C file: they would add bytes to your object that the original keeps elsewhere. Use an `extern` array with the existing symbol name instead. Linking `.data` and `.rodata` from C is not your job and not in the loop for now (issue 0082).
- **`.bss` is defined in C** (`tools/bss.py`, issue 0082): many zero-initialised globals have a definition, either in the file of a function that uses them or in a `<directory>_bss.c` file. Use the existing declaration; never define a `.bss` variable yourself. Leave `.bss` definitions, their `#pragma section BSS_RUN...` lines and each file's `.bss` range in `delinks.txt` as they are, also in a file you edit or rename.
- **Struct sizes now set RAM layout.** A struct that a `.bss` definition uses (`grep -rn 'StructName ' src/` finds a definition without `extern`) must keep its size: add fields by replacing padding, never grow or shrink it. A size change moves every later variable in RAM; `make release` fails when code points at one of them, which is nearly always. If your code shows such a struct or a `.bss` variable has another size, put it in NOTES.
- **Shared headers first:** before defining a struct, look in `include/game/` for a header that already describes the data (grep for the symbol name or the address). The CO table (`gCoTable`, `CoRecord`, `CoPowerBlock`, `CoStatRow`) is in `game/co_data.h`; the map header table (`Ov000_gMapHeaders`, `MapHeader`) in `game/map_header.h`; players (`Player`, reached as `data_027c027c->unk_48[data_027c0284]->players`) in `game/player.h` and `game/Unk_02080440.h`; unit types and `Ov000_gUnitStats` in `game/unit_stats.h`; units on the map in `game/map_unit.h`. Add fields to those headers; never make a local copy. Rename an `unk_XX` field there only when your code shows what it is.
- **Structs:** when the function reads fields through a pointer, define a struct. Put it in a header `include/game/<StructName>.h` so other files can share it (structs that are only reached through each other may share one header), with `unk_XX` names (XX = hex offset) for fields whose meaning is unclear and explicit padding (`u8 pad_XX[N];`) for gaps. Name the struct by what it is if you can tell; otherwise `Unk_<address of your function>`. Do not use pointer arithmetic with magic offsets when a struct works.
- **Types:** use unsigned (`u8`, `u16`, `u32`) unless the code shows a sign: `ldrsb`/`ldrsh`, `asr`, or a signed compare (`blt`, `bgt`, ...). Parameters often have to stay `int` to match, because the compiler then emits the narrowing itself.
- **Compiler (mwccarm 2.0-72, `-O4,p`):** old and literal. Things that change codegen: declaration order of locals, `for` vs `while`, `if` vs a one-case `switch` (2.0 turns short tails into conditional instructions), temporaries, signed vs unsigned, `u8`/`s16` locals vs `int`, `x << n` vs `x * 2^n`, splitting or joining expressions, the order of `&&` terms. No `-ipa`, so each function compiles on its own. Calls to ITCM functions from main compile to `ldr ip, =f; bx ip` by themselves.
- **Shapes that fixed stubborn diffs before:** `docs/workflow.md`, "Near misses: the cookbook", lists every rewrite that matched a near miss so far, grouped by what the diff shows (a reload the original makes, registers swapped, spill slots, instruction order, conditional moves), with the function each one matched. Read it when a function stalls below 100%, before you try variants of your own.
- Thumb functions are SDK assembly: skip them (the orchestrator should not send them).

## 4. Diff until 100%

```sh
python3 tools/fn.py diff NAME
```

Lines marked `|` differ. The last line gives their count and objdiff's percentage (`NAME: 1 differ, 99.907776% match`). Exit 0 means 100%. Budget: about 25 compile-diff rounds. If you are stuck below 100% after that, stop (see step 8).

Before you change C to fix a differing line, find out which statement made it:

```sh
python3 tools/fn.py lines NAME [FILE.c]
```

It prints our compiled code in runs, each under the C line mwcc assigned it to, with call and literal-pool targets by name. Look up the instructions on our side of each `|` line there and edit that statement, not a guess. mwcc's scheduler mixes neighbouring statements, so one line can show up in several runs and a load sits where it ended up, not under the statement that uses it; a statement that makes no code (folded into the next one) has no run. The original has no line table: use `diff` for its side. Pass FILE.c to see a scratch variant.

Keep the attempt with the fewest differing lines, not the highest percentage: objdiff charges one wrong opcode as much as 12 wrong registers, so a version that differs only in registers can score higher and still be further from a match (issue 0067). Compare percentages only between attempts with the same count. When two attempts each fix a different part of the function, try combining those parts: CampaignMap_Init got down to 1 differing line by taking each half from a different attempt.

Iterate fast with `python3 tools/fn.py check FILE.c NAME` (milliseconds, no `delinks.txt` entry needed); confirm with `diff`.

Before you report `partial`: rule out the bugs in step 1 of the cookbook's "Order of work" (`docs/workflow.md`), copy a matched twin's shape if there is one, and run the pragma sweep before hand variants: `python3 tools/pragmas.py FILE.c NAME` (the original switched optimisations per file; a match with `--apply` keeps the pragma, with a comment). Then run `python3 tools/permute.py FILE.c NAME --budget-seconds 120`. It tries declaration orders, statement swaps and local types. If its best variant is 100%, apply it (`--apply`), tidy the names, and finish as a match.

Common fixes: register mismatch → change local order or types; extra/missing instruction → expression shape, casts, signedness; branch shape → `if`/`switch`/loop form; constant pool order → order of first use.

## 5. Make it readable

Once it matches, improve names and types without losing the match (rerun `diff` after each change):

- Rename the function when its behavior is clear from the code and its callers: `python3 tools/fn.py rename func_02080440 Player_ResetCursor`. If the file is still named after the address, rename moves it too. Rename rewrites your C file, so read it again before editing. Same for data you understand (`data_0229d8c4` → `gCursorState`). Game names follow `Area_Verb` or `VerbObject` style. Keep the address name if you are guessing.
- Parameter and local names describe use. No `int self`, no `v1`.
- Library code (`src/sdk/`, `src/arm7/sdk/`): a library-style name (a prefix such as `OS_`, `CARDi_` or `REG_`, or `WM` followed by a capital) is allowed only if it is in `tools/pnc_sdk_names.txt` (names with a public source; tenet 10). Never use a name you remember from an SDK that is not on that list: give the function a name of our own in the `Subject_Verb` style (`CardSave_FillGeometryFromType`). `make release` refuses unlisted names in files and commit messages.
- Comments only for why the code has an odd shape ("needed to match: ...") or for game facts that are not obvious.

## 6. Finish

```sh
python3 tools/fn.py finish NAME       # marks the file `complete`; the link now uses your object; formats the file
make format                           # if you changed headers or other files under src/ or include/
```

Then stage the change, run the review pass (`.claude/skills/tenets-review/SKILL.md`) on it, fix what it finds, record the review, and commit. The review is not optional: `land_self.py` refuses (exit 5) a branch with a commit that changes `src/` or `include/` and has no review stamp. The stamp needs the review's findings file (a `<path>: <findings>` line for each changed file under `src/` and `include/`; the skill's step 4 shows the form) and fits the exact diff, so stamp after your last fix and commit right away:

```sh
git add -A config src include
F=$(git rev-parse --git-path review-findings.txt)    # written during the review
python3 tools/review_stamp.py --findings "$F"        # stamps the staged change
git commit -m "Decompile <NewName> (<module> 0x<addr>)" -m "<one or two lines on what the function does, and anything odd>" -m "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
"
```

The hooks run `make release`; the commit only exists if the ROM still matches. In a worktree they leave `PROGRESS.md` alone; mainline writes it when your commit lands.

## 7. Land on mainline

Other agents land on `mainline` while you work. Land your branch yourself, once, after your last commit, while you still know your code (each landing costs a full `make release` and the queue serves one branch at a time, so batch your commits into one landing):

```sh
python3 tools/land_self.py
```

It waits for its turn in a landing queue shared by all agents (it prints `waiting for the landing queue, held by ...`; that is not a hang), then replays your commits onto the current `mainline` (`tools/rebase_onto_mainline.py`, which uses `tools/apply_commit.py`), runs `make release`, writes `PROGRESS.md` into your last commit, and fast-forwards `mainline` to your branch under a lock. If `mainline` moved meanwhile, it replays again (up to 15 times). It prints `LANDED: <hash>` on success.

The replay merges shared headers by meaning: where another agent already named a field you named too, mainline's name stays and your C is rewritten to it. If another struct also has a field of your old name, it does not rewrite anything and stops with the uses (`file:line`): change the ones that read the merged struct. Enum constants and `#define`s of both sides are kept; one name with two values stops the replay. It prints what it merged and renamed, and each replayed commit runs `make release`.

Run it in the foreground with a long Bash timeout (600000 ms). A run cut off by the timeout keeps going in the background, also while it waits in the queue. A second `land_self.py` or `rebase_onto_mainline.py` in the same worktree then refuses to start (exit 4) and prints the lock file; wait for the first with `flock <that file> true`, then read `git log -1` to see whether it landed. Never stop a process you did not start (no `pkill`, no `kill` by pattern): other agents land at the same time. Do not wait with `pgrep -f <pattern>` loops; they match themselves.

Exit codes: 0 landed; 1 the replay stopped; 2 `make release` failed; 3 gave up (mainline kept moving, or the main checkout stayed busy for 10 minutes); 4 busy (a replay is open in this worktree: finish it with `--continue` or `--abort`; or another run holds the worktree lock: wait as above); 5 commits without a review stamp: it lists them; run the tenets review on each (`git show <hash>`), commit any fixes (reviewed and stamped too), then `python3 tools/review_stamp.py --findings "$F" <hash>...` and run `land_self.py` again. `Propagate ...` commits that only add files under `src/`, and `WIP ...` commits that leave `include/` alone, need no stamp.

On 1 (conflict markers, a clash it reports such as "kept mainline's Task.unk_08 as `void *unk_08;`", a failed build), fix the named files: use mainline's names and types, cast where a mainline field has a vaguer type than yours, never undo another agent's header change. Check `python3 tools/fn.py diff NAME` is still 100%, then:

```sh
git add -A config src include
python3 tools/rebase_onto_mainline.py --continue
python3 tools/land_self.py
```

A commit you fixed by hand during `--continue` loses its review stamp (`--continue` prints `fixed by hand, so it needs a new tenets review`), and `land_self.py` exits 5 on it. Review the new commit (`git show <hash>`), write its findings, stamp it with `python3 tools/review_stamp.py --findings "$F" <hash>`, and run `land_self.py` again.

On 2, fix the build and commit the fix, then run `land_self.py` again. On 3, report it; the orchestrator lands your commit. `rebase_onto_mainline.py --abort` puts the branch back as it was if you cannot finish a replay. Do not amend commits yourself; `land_self.py` is the only thing that amends.

## 8. Report

End with exactly this block (the orchestrator parses it):

```
RESULT: matched | partial | skipped
FUNCTION: <original name> -> <final name>
COMMIT: <full hash after landing (several: space-separated), or none>
LANDED: <mainline hash that land_self.py printed, or none (say why in NOTES)>
BEST: <n> differ (<percent>%), from fn.py diff's last line, for example 1 differ (99.91%)
FILE: <src path>
NOTES: <one to three lines: what the function does, what blocked you, names you were unsure about>
```

On `partial` or `skipped` (after the `permute.py` run), do not commit. Leave the worktree as it is.

## ARM7 functions

Only when your group names `func_arm7_XXXXXXXX` functions (module `arm7`). The steps above apply, with these differences (`docs/workflow.md`, "ARM7"):

- The file goes under `src/arm7/sdk/`: `python3 tools/fn.py add func_arm7_038076e8 src/arm7/sdk/func_arm7_038076e8.c`, or a topic directory there once the code shows one.
- Files under `src/arm7/` compile with `-O4,s -proc arm7tdmi -interworking` (built for size; ARM9 uses `-O4,p`). `fn.py check` and `diff` use those flags by themselves.
- All of ARM7 is library code (sound, wireless, touch screen, power). Its data is its own: do not reach for the structs in `include/game/`.
- `fn.py pick --arm7` lists ARM7 candidates. `python3 tools/propagate.py --module arm7` and `python3 tools/trivial.py --arm7` handle ARM7 twins and short functions; without those flags they skip ARM7.
