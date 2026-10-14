# 0066: Map event scripts are bytes in ov001

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

The unpacked map files cover each map's header, tiles and units, but not its scripts. `MapHeader.script`, `termsScript` and `hintScript` hold symbol names (`data_ov001_02364af0`), and the data they point to is still copied from the ROM as undelinked bytes in ov001. A map's events (dialogue, reinforcements, win conditions) cannot be read or changed in the map files.

## What we know

- ov001's `.data` (0x023522a0-0x0237a5a0, 164,608 bytes) holds only map scripts, in three kinds of object, each a typed array in `src/game/ov001/ov001_data_*.c` with one record per line of its `inc.c` file, opcodes and trigger kinds written by their `EVENT_OP_*` and `EVENT_TRIGGER_*` names (2026-10-14):
  - 64 `MapScript`s (`include/game/map_script.h`, 0x1c bytes): seven trigger lists. The runners: +0x00 at turn start (`Game_State6_PlayThemeAndScripts`), +0x04 in game state 11 (`MapScript_RunList04`), +0x08 with a selected player 1 unit (`MapScript_RunList08`, which starts the task `data_ov000_022f6bd8` when a block ran), +0x0c with a unit that finished its action (`MapScript_RunList0c`), +0x10 with an id and a unit (`MapScript_RunList10`), +0x14 from `Game_EnterState20`, +0x18 from `Game_TickPlayerTimer`. An unused situation points to `Ov001_gNoTriggers` (only `EVENT_TRIGGER_END`, 180 references) or is NULL; +0x18 is NULL in every map.
  - 207 `EventTrigger` lists (`include/game/event_script.h`, 8 bytes per entry): blocks of conditions, each closed by a run entry, the list closed by `EVENT_TRIGGER_END`. Conditions: day and player, unit type, game flag set or clear, id, a test function (the `Ov001_*` functions at the start of ov001), normal or hard mode. Each comes in three kinds: the first map state, the other (`+ EVENT_KIND_SECOND_MAP`), or both (`+ EVENT_KIND_BOTH_MAPS`); kinds 23-25 are the run entry in the same three forms. Map scripts use only the first form and, for the test and run kinds, the second. The runner is `EventTriggers_Run`.
  - 540 event scripts: arrays of 16-byte `EventScriptCommand`s, 9,603 commands in all. The opcode indexes the 93 handlers of `data_021585c0` (`EventScript_StepSlot`); `EVENT_OP_*` in `event_script.h` names them after their handlers. The word at +4 (`u`) is a function (`EVENT_OP_CALL`, 296 of them `MenuDepth_Push` and `MenuDepth_Pop`), a jump target, a task script or an address; it is never a plain number. The word at +0xc (`arg`) is a number (frames, text id, song, count) except for `EVENT_OP_JUMP_IF_TRUE`, where it is a test function. The halfwords at +8 and +0xa hold players, pictures, flags and tiles. A script ends with `EVENT_OP_END` (537 times; opcodes 2, 3 and 22 have the same handler but no script uses them) or loops with `EVENT_OP_JUMP`.
- Where scripts come from: 64 `MapHeader.script` values, 28 `termsScript` and 36 `hintScript` values (64 scripts in all), 29 `CampaignMission.script` values in main (the world map starts them, `CampaignMap_StartMissionScript`), the run entries of the trigger lists, and jumps inside scripts (`EVENT_OP_JUMP`, `JUMP_IF_TRUE`, `JUMP_IF_FLAG_SET`, `JUMP_IF_COS`, and `EVENT_OP_CALL_UNLESS_SKIPPING`'s function). Every jump target is the start of its own object: there is no symbol inside an object.
- Opcodes in use: 44 of 93. The most common: `CHANGE_PICTURE` (2,252), `PRINT_WINDOW_TEXT` (2,315), `SHOW_PICTURE` (953), `LOAD_ARMY_PALETTE` (952), `SLIDE_IN` (771), `END` (537), `CALL` (403), `SKIP_UNLESS_CO` (388).
- Up to 10 scripts run at once (`data_022977f0`). Code in main reads a command through three views: `EventScriptCommand`, `EventScriptDataCommand` (the store and copy commands) and `Unk_0201d320_Entry`; the last could become `EventScriptCommand`.
- dsd had cut three scripts in the middle of a command, at symbols that came from other overlays' relocations to the same addresses (0x02365ea8, 0x0236d644, 0x023700c0). Those symbols are gone and the scripts are whole. The last script, `data_ov001_0237a518`, is 0x70 bytes; the 24 zero bytes after it are the linker's pad of ov001's `.data` to 32 bytes.
- Named so far: `Ov001_gNoTriggers`, `Ov001_gSlideInScript`, and the shared scripts that defeat a team or every team but one (`Ov001_gDefeatTeam1Script`, `Ov001_gDefeatOtherTeams2Script` and so on). Objects that a map file names (`MapHeader.script`, `termsScript`, `hintScript`) keep their `data_ov001_` names: the unpacked map files hold those names, and a rename would break `maps.py pack` until each builder unpacks again.

## Plan

1. ~~Map the ov001 ranges the scripts and trigger lists use, and the symbols other code refers to inside them.~~ Done: every object is typed.
2. ~~Name the trigger kinds and the opcodes from their handlers.~~ Done for the opcodes with a named handler (`EVENT_OP_*`). Opcodes 13, 35-38, 40 and 61 only move on, 5 and 39 stay where they are, 56 has no handler; 66, 68 and 69 are variants of 65, 67 and 70 whose difference is not understood.
3. Carry each map's scripts in its map file. A plan, not built:
   - `maps.py unpack` walks from the header as above: `MapScript`, then its lists, the scripts their run entries name, and every jump target, and writes a `script` section into the map file: `lists: {list00: LABEL, ...}`, `triggers: {LABEL: [[KIND, param, day, target], ...]}` and `scripts: {LABEL: [[OP_NAME, u, unk_08, unk_0a, arg], ...]}`, with opcodes and trigger kinds by their `EVENT_OP_*` and `EVENT_TRIGGER_*` names (maps.py already reads `#define`s from headers) and pointers by symbol. Labels are the object's symbol, so the existing names keep working.
   - An object that several maps use (the terms and hint scripts of map variants, `Ov001_gNoTriggers`, the defeat scripts, and every object referred to from main) goes into one shared file, `assets/maps/scripts.yaml` under `extracted/`, and the map files name it.
   - `maps.py pack` writes each object's `inc.c` from the YAML in place of `data_inc.py`'s row, so `src/game/ov001/ov001_data_*.c` keep the objects, their order and their files. The ov001 rows of `data_inc.txt` then go. A script that grows only moves the objects after it, which link by symbol.
   - Adding or removing an object needs a definition in the C files. A later step can write those files from the YAML too, with `tools/mwcc_order.py` for the order inside each file, or put each map's objects in one file of its own.
   - `maps.py check` packs the unpacked files and compares ov001's `.data` with the ROM byte for byte.

## Done when

The scripts of every map are unpacked next to the map files as named triggers and commands, the build makes ov001's script data from them, and `make release` passes.
