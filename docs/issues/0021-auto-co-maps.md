# 0021: Name the Auto CO special case for Lightning Strikes and Ring of Fire

- **Status:** closed
- **Area:** decomp
- **Opened:** 2026-10-08
- **Milestone:** M2
- **Closed:** 2026-10-08. `MAP_LIGHTNING_STRIKES` and `MAP_RING_OF_FIRE` moved to `game/map_header.h` as the Auto CO missions; the menu checks are `BattleMenu_AutoCoOffHidden` and `BattleMenu_AutoCoOnHidden`, the table `gBattleMenuItems` (`MenuItem` in `game/menu_item.h`), and `unk_92` is `slotControllers`.

## Problem

`src/game/main/map/map_init_battle.c` defines `MAP_LIGHTNING_STRIKES` (0xea) and `MAP_RING_OF_FIRE` (0xf5) with the comment "Campaign missions played on two maps at once". Five campaign missions use two maps, so the comment is wrong. What sets these two apart is that they offer the "Auto CO On" / "Auto CO Off" battle menu items. With those, the player can play the other front instead of leaving it to the AI.

## Notes

Campaign maps with a second map (`variantMapIds[0]` nonzero in `Ov000_gMapHeaders`): 0xe8 Victory or Death, 0xea Lightning Strikes, 0xee Omens and Signs, 0xf5 Ring of Fire, 0xf8 Means to an End. The second maps are 0xfc-0x100.

Only three functions in ARM9 compare against 0xea and 0xf5: `Map_InitBattle`, `func_020be768` and `func_020be908`.

- `func_020be768` and `func_020be908` sit in a menu table at main `0x02165e8c` (0x20 bytes per entry). Their entries have text ids 0xc0000067 "Auto CO Off" and 0xc0000066 "Auto CO On". Both return 1 (item off) unless all of these hold: `gameMode == 0`, the map is 0xea or 0xf5, and the other front is still running (other state's `unk_1c4->unk_30 == 0`). In that case they read `players[current].controller` in the other state and return `== AI` and `!= AI`.
- The same table holds Strike, Assault, General and Defense (0xc0000062-0x65; `func_020be560`, `func_020be358`, `func_020be150`, `func_020bdf48`). These call both checks above and test `Player.unk_2e == 3`.
- In `Map_InitBattle`, the second map's `unk_92` is copied from `players[i].controller` on these two maps. On other maps, every nonzero entry is set to 2 (AI).
- `func_020e8ae4` returns `visionRanksOff == 1` and turns this special case off. The field name is a guess.
- That "Auto CO" means "the AI plays this front" comes from the menu text and from memory of the game. It has not been checked in play.

## Done when

- The defines and their comment in `map_init_battle.c` say these are the Auto CO missions. A shared header could hold them if more code needs them.
- `func_020be768` and `func_020be908` have names (for example `BattleMenu_AutoCoOffShown` / `BattleMenu_AutoCoOnShown`), and the menu table has a name.
- `unk_92` has a name, or its comment says it holds a `PLAYER_CONTROLLER_*` value per slot.
