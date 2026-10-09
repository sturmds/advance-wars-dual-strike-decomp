# 0022: Player.armyStyle and armyColour comments are wrong

- **Status:** closed
- **Area:** decomp
- **Opened:** 2026-10-09
- **Milestone:** M2

## Problem

The comments mix up army style and army colour. From the code (func_020c1444 and func_020c672c read as assembly; not yet decompiled):

- `Player.armyColour` (+0x19) is the palette. func_020c1444 sets it at battle start from `state->army[p]` (the map header, through Map_LoadHeaderSetup). func_020c672c returns `&data_021405b8[armyColour - 1]`, 32-byte rows; rows 0-4 decode to red, blue, green, yellow and dark purple (Orange Star to Black Hole).
- `Player.armyStyle` (+0x2d) is the sprite set of the front CO's home army: func_020c1444 sets it to `CoRecord[front CO].armyStyle + 1`; func_020fed58 writes it the same way, probably on a CO change. Player_GetArmyStyle (was func_020e0120) returns it; Map_GetTileArmy and the ov000/ov002 drawing code call it.
- In War Room the colour is always Orange Star while the style follows the CO.

## Comments to fix

- `include/game/player.h`: the `ARMY_BLACK_HOLE` comment says "Army colour (Player.armyColour, MapHeader.army)". `armyStyle` needs "sprite set of the front CO's army, CoRecord.armyStyle + 1".
- `src/game/main/map/map_get_tile_army.c`: keep "style", say sprite set, not colour. `MapHeader.tileArmy` (+0x9c) is a style too.
- `include/game/co_data.h`, `CoRecord.armyStyle` ("0-4: OS, BM, GE, YC, BH"): add "sprite set".

## Notes

Found by sturmds and a planning session while looking at an AW2-style War Room mod (issue 0023). Decompiling func_020c1444, func_020fed58 and the readers of +0x19 would confirm the palette reading.

2026-10-09: the comments listed above are fixed (renames pass before batch 10). The issue stays open until func_020c1444 is decompiled.

2026-10-09: func_020c1444 is decompiled as Players_ApplyBattleSetup. It sets armyColour from state->army[slot] and armyStyle from the front CO in state->playerCos (CoRecord.armyStyle + 1), as described above. func_020fed58 is committed as a 97% WIP; it writes armyStyle from the campaign map header's front CO.

## Done when

The comments above say style for sprite set and colour for palette, and func_020c1444 is decompiled.
