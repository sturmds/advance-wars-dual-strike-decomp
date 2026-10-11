#ifndef GAME_FIGHTERTASK_H
#define GAME_FIGHTERTASK_H

#include "types.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_fighter.h"
#include "game/menu_state.h"
#include "game/battle_hit_list.h"
#include "game/BattleCellTask.h"

/* Task of one side's fighter in the ov002 battle animation, made by
 * Battle_StartFighterTask. The first 0x2c bytes belong to the task system. */
typedef struct FighterTask {
    u8 pad_00[0x2c];
    BattleCellTask *fighter;
    BattleHitList *steps;              /* the side's hit list; count is how many entries it has */
    BattleSpriteTask *sprite;          /* the sprite task last made for the fighter */
    BattleAnimData *anim;              /* BattleSideData.cellPositions of the side, read with another layout */
    s32 step;                          /* next step of the list */
    u16 scale;                         /* 8.8 fixed point; BattleCellTask.scale */
    u8 side;                           /* 0 or 1 */
} FighterTask;

#endif
