#ifndef GAME_BATTLE_LAYOUT_H
#define GAME_BATTLE_LAYOUT_H

#include "types.h"
#include "game/battle_fighter.h"
#include "game/battle_hit_list.h"

/* Position of one HP cell on the battle screen. */
typedef struct BattleCellPos {
  s16 x;
  s16 y;
  s16 z;
} BattleCellPos;

/* The two record layouts BattleSideData.layout points to; the tables hold one or two records.
 * BattleCellLayout.base and BattleFighterLayout.offsets start at the same place, and
 * BattleCellTask_GetCellOffsetX reads either. A BattlePoint here holds the two offsets that
 * func_ov002_02356604 and BattleSpriteTask_Create take after the position. */

/* A side with BattleSideData.unk_48 at 0: one task for each HP cell. */
typedef struct BattleCellLayout {
  BattleCellPos cells[BATTLE_CELL_COUNT]; /* BattleCellTask_Create, by cell */
  BattlePoint weaponOffsets[2];           /* by BattleSide.usesSecondary */
  BattlePoint base;                       /* shared by all objects */
  BattlePoint unitOffsets[BATTLE_CELL_COUNT]; /* one for each cell */
  BattlePoint weaponStyleOffsets
      [2]
      [BATTLE_CELL_COUNT]; /* by BattleSide.usesSecondary, then by BattleSide.armyStyle - 1 */
} BattleCellLayout;

typedef char
    BattleCellLayout_size_check[sizeof(BattleCellLayout) == 0x66 ? 1 : -1];

/* A side with BattleSideData.unk_48 set: one fighter task. */
typedef struct BattleFighterLayout {
  BattleCellPos pos;   /* where the fighter task starts */
  BattleCellPos shift; /* Battle_DrawSpriteCells moves the sprite by it */
  u16 scale;           /* 8.8 fixed point */
  BattlePoint
      steps[BATTLE_CELL_COUNT + 1]; /* by step of the hit list, also the pose of
                                       Battle_PutFighterSprite; Battle_UpdateCueSpawner
                                       reads the last */
  BattlePoint offsets[BATTLE_CELL_COUNT]; /* by cell */
} BattleFighterLayout;

typedef char BattleFighterLayout_size_check[sizeof(BattleFighterLayout) == 0x3a
                                                ? 1
                                                : -1];

typedef union BattleLayout {
  BattleCellLayout cell;
  BattleFighterLayout fighter;
} BattleLayout;

/* Position and speed of a moving piece of the battle animation. */
typedef struct BattleMotion {
  s32 unk_00;
  s16 x;
  s16 y;
  s16 speedX;
  s16 speedY;
  s16 velocityX; /* 8.8 fixed point */
  s16 velocityY;
  u16 delay; /* frames left before it moves */
  u8 pad_12[2];
} BattleMotion;

#endif
