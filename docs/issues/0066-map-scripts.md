# 0066: Map event scripts are bytes in ov001

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

The unpacked map files cover each map's header, tiles and units, but not its scripts. `MapHeader.script`, `termsScript` and `hintScript` hold symbol names (`data_ov001_02364af0`), and the data they point to is still copied from the ROM as undelinked bytes in ov001. A map's events (dialogue, reinforcements, win conditions) cannot be read or changed in the map files.

## What we know

- `MapHeader.script` points to a `MapScript` (`include/game/map_script.h`): four lists of `EventTrigger` at +0x00, +0x10, +0x14, +0x18, run at different moments (turn start, `MapScript_RunList10` with an id and a unit, `Game_EnterState20`, `Game_TickPlayerTimer`). The headers point to 64 different `MapScript`s. What +0x04-+0x0f holds is not known.
- An `EventTrigger` list (`include/game/event_script.h`, 8 bytes per entry) is blocks of conditions, each closed by `EVENT_TRIGGER_RUN`, ending with `EVENT_TRIGGER_END`. Conditions: day and player, unit type, game flag set or clear, id, a test function, normal or hard mode. Each comes in three kinds: one map state, the other, or both. Kinds 24 and 25 are not named yet.
- A run entry points to an event script: an array of 16-byte `EventScriptCommand`. The opcode indexes the handler table `data_021585c0` (93 entries, `EventScript_StepSlot`), and the word at +4 is a handler, an address or another script (`EventScriptDataCommand`). About 47 handler files are in `src/game/main/script/`. Up to 10 scripts run at once (`data_022977f0`).
- The `termsScript` and `hintScript` pointers are event scripts started directly (28 and 36 different scripts).
- The scripts hold pointers into main (handlers, test functions, data) and to text, so they must be written with symbols, like the header table, for the build to keep matching.

## Plan

1. Map the ov001 ranges the scripts and trigger lists use, and the symbols other code refers to inside them.
2. Name the trigger kinds and the opcodes from their handlers.
3. Add a `script` section to each map file (triggers, then each script as a list of commands by name), packed into C the way `tools/maps.py` packs the header table, and give the ranges to C files in ov001's `delinks.txt`.

## Done when

The scripts of every map are unpacked next to the map files as named triggers and commands, the build makes ov001's script data from them, and `make release` passes.
