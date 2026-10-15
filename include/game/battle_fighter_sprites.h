#ifndef GAME_BATTLE_FIGHTER_SPRITES_H
#define GAME_BATTLE_FIGHTER_SPRITES_H

#include "types.h"
#include "game/menu_state.h"
#include "game/BattleSideData.h"

#define BATTLE_OFFSET(field) ((u32) & ((MenuState*)0)->battle.field)

/* The side's data (BattleSideData) as the ov002 motion steps read it. Needed to
 * match: the offset into gMenuState is added at run time. */
#define BATTLE_SIDE_DATA(side) \
  (*(BattleSideData**)((u8*)&gMenuState + (side) * 4 + BATTLE_OFFSET(unk_27c)))

/* The BattleEntry of a side's cell. Needed to match, like BATTLE_SIDE_DATA. */
#define BATTLE_ENTRY(side, cell)                                   \
  (*(BattleEntry*)((u8*)&gMenuState +                              \
                   (side) * sizeof(gMenuState.battle.unk_310[0]) + \
                   (cell) * sizeof(BattleEntry) + BATTLE_OFFSET(unk_310)))

/* The BattleEntry of a fighter's side and cell, whose sprite tasks the motion
 * steps move. */
#define BATTLE_FIGHTER_ENTRY(fighter) \
  BATTLE_ENTRY((fighter)->side, (fighter)->cell)

#endif
