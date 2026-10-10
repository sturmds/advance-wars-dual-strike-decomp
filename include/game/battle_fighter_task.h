#ifndef GAME_BATTLE_FIGHTER_TASK_H
#define GAME_BATTLE_FIGHTER_TASK_H

#include "types.h"
#include "game/battle_fighter.h"
#include "game/BattleSpriteTask.h"
#include "game/menu_state.h"
#include "game/battle_hit_list.h"
#include "game/BattleSideData.h"

/* A task of the ov002 battle animation that follows one BattleFighter.
 * The first 0x2c bytes belong to the task system. */
typedef struct BattleFighterTask {
    u8 pad_00[0x2c];
    BattleFighter *fighter;
    BattleHitList *hitList;            /* BattleScreenWork.hitLists of the side */
    BattleSpriteTask *sprite;          /* the sprite task the script waits on */
    const BattleCellPos *cells;        /* BattleSideData.cellPositions of the side */
    u32 counter;
    u16 scale;                         /* 8.8 fixed point; set by the creator */
    u8 side;
} BattleFighterTask;

#endif
