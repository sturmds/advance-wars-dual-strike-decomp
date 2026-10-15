# Readability passes

Matching C that only a compiler can follow is half done (tenets 2 and 11). This page is the method for making the code easier to read across the whole tree without losing a match: what the rules are, which tool does each step, and how each step is checked. Issue 0089 tracks the work.

## Rules

1. **One declaration per name.** Every function, global variable and shared macro is declared once, in a header under `include/`. A C file includes headers; it does not copy declarations. Searching for a name finds its definition and one declaration.
2. **Headers by topic.** A name's header is the one a reader would guess:
   - a variable goes in the header that defines its type (`data_027c0280` in `game/Unk_02088d04.h`, `gMenuState` in `game/menu_state.h`);
   - a function goes in the header of the source file that defines it when that file holds a topic (`src/game/main/ai/ai_show_action.c` and `include/game/ai/ai_show_action.h`), else in an existing header for what it works on (`Oam_SetAffineParams` in `game/oam_buffer.h`), else in the header of its directory (`src/game/main/math/` and `include/game/math.h`, `src/game/main/screen/` and `game/screen.h`);
   - a macro that many files define the same way goes in the header of the struct it reads (`MAP_STATE` in `game/Unk_02080440.h`).
3. **Code of one topic in one file.** Neighbouring functions that belong together share a source file, in reverse address order, with its header (`docs/workflow.md`, "Merging files").
4. **One name for one thing.** The same expression under two names (`STATE` and `GAME`) becomes one name.
5. **A difference stays only when the match needs it, and says so.** A declaration with another type than the header's stays in its C file when the compiler emits other code with the header's type, listed in `config/AWRE/local_decls.txt` with the reason. A macro written in an odd form keeps a comment saying it is needed to match.

## Checks

Every step here must leave every function's code as it was:

```sh
python3 tools/objcmp.py snap      # on a build of the tree before the step
...                               # the step
make && make check
python3 tools/objcmp.py compare   # lists any function whose code changed
```

`make check` says whether the ROM matches; `objcmp.py compare` says which function changed when it does not, and catches a change in a function whose bytes happen to land the same. A declaration does not produce code, so a moved or removed declaration can change a caller only through its type: the width of an argument (`s16` against `int`) or of the return value.

`python3 tools/decls.py check` lists the C files that copy a header's declaration and the names two headers declare. `make release` fails on any not listed in `config/AWRE/local_decls.txt`, so new code keeps the rules.

## Steps, in order

1. **Give a name a home** (`decls.py declare HEADER NAME... [--dir src/DIR]`). Writes each declaration into HEADER (a header's declaration as it is, else the definition's prototype), takes it out of other headers, removes the C files' copies, and adds the include where a file used the name through another header. It moves a declaration without changing its type, and leaves a name where it is when a file that would get the header declares one of its names otherwise, or defines one of its macros or types otherwise; it says which file.
2. **Remove the copies** (`decls.py dedupe`). Removes each C file's copy of a header's declaration and includes that header, under the same conditions.
3. **Make the copies agree** (`decls.py reconcile --write`, then `dedupe`). Gives each differing copy the header's text, a declaration per file per round, keeps those that compile and change no code, and lists the others in `local_decls.txt`. Needs `objcmp.py snap` first.
4. **Find the one prototype** (`decls.py unify NAME [--variant PROTO] [--apply]`). When callers declare a callee with other widths (`s16` against `int`, `u8` against `u32`), tries each prototype as the only one, for the definition, the header and every caller, and prints what each fixes and breaks. The original was built with one of them, and it changes nothing: `Ai_CanEndMoveAt(s16, s16)`, `Unit_IsCurrentPlayers(s16)`, `UnitType_GetAttackKind(u8)` and `AiUnit_PerformAction(s16, s16, u8, u8, u8)` were found this way, against definitions written with `int`. When several callees change together (`AiBuild_GetTargetGroup`, `AiProfile_AllowsBuild` and `AiBuild_RateAttackerVsTarget` all take a `u8` unit type), try them together.
5. **One macro for one expression** (`decls.py macro HEADER NEW OLD...`). Compares the bodies after expanding the file's other macros; defines NEW in HEADER with the body most files use, renames OLD to NEW where it means that body, and leaves the other bodies alone, listed.
6. **Merge topics** (`merge_files.py OUT.c FILE.c...`). Merges neighbouring files into one with a header, and replaces the callers' copies with the header. A merge can show that two functions were compiled with different prototypes of one callee; find its one prototype with step 4 first. When no prototype works for both, the functions were in different original files: move one out (`merge_files.py --split FILE.c FUNC [--rest REST.c]`) and keep its declaration, listed in `local_decls.txt` (`AiEmptyTransport_GoToFootUnit` passes an `int` to `RangeMap_FillMoves` where `RangeMap_FillUnitMoves` needs an `s16`; `func_02098e7c` passes an argument that `AiTurn_IsPhase3` ignores).

Go from the names most files copy to the least: `decls.py check --list` sorted by name shows them. Look at what a step chose before committing it: a tool picks the header that declares a name first, which is not always the right home; give the name a home with `declare` first.

## Limits

- mwccarm applies an optimisation `#pragma` to every function of a file, also between `#pragma push` and `#pragma pop`. A function that needs one keeps its own file; `merge_files.py` refuses it.

- A function whose prototype names a type that only a C file defines (`Unk_020e47cc` in `Screen_StartLayer0Blend`) cannot go in a header until the type does.
- Where the original source files began and ended is not known, so topics are our grouping.
- `objcmp.py` compares code. Data order and data values are checked by `make check` only.
