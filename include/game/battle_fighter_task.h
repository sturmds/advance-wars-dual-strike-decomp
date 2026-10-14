#ifndef GAME_BATTLE_FIGHTER_TASK_H
#define GAME_BATTLE_FIGHTER_TASK_H

#include "types.h"
#include "game/battle_fighter.h"
#include "game/BattleSpriteTask.h"
#include "game/menu_state.h"
#include "game/battle_hit_list.h"
#include "game/BattleSideData.h"
#include "game/BattleCellTask.h"

/* A task of the ov002 battle animation that follows the first cell task of a
 * side: Battle_StartFighterTask, func_ov002_0235f02c and func_ov002_0235f3c0
 * make them from three scripts. The first 0x2c bytes belong to the task system. */
typedef struct BattleFighterTask {
  u8 pad_00[0x2c];
  BattleCellTask* fighter; /* cell task of the side's first cell */
  BattleHitList* hitList;  /* BattleScreenWork.hitLists of the side */
  BattleSpriteTask*
      sprite; /* the sprite task last made; the scripts wait on it */
  const BattleLayout* layout; /* BattleSideData.layout of the side */
  s32 step;                   /* next entry of hitList */
  u16 scale;                  /* 8.8 fixed point; BattleCellTask.scale */
  u8 side;                    /* 0 or 1 */
} BattleFighterTask;

#endif
