#ifndef GAME_BATTLE_LAYOUT_H
#define GAME_BATTLE_LAYOUT_H

#include "types.h"

/* A y, x pair of screen pixels. */
typedef struct BattleOffset {
  s16 y;
  s16 x;
} BattleOffset;

/* Positions of the HP cells and offsets of one side of the battle animation (ov002).
 * BattleSideData.cellPositions points to it: the first 0x1e bytes are the five cell
 * positions (BattleCellPos), the rest is read through this struct. */
typedef struct BattleLayout {
  s16 x; /* start position of a fighter object */
  s16 y;
  s16 z;
  u8 pad_06[6];
  u16 scale; /* 8.8 fixed point */
  u8 pad_0e[0x1e - 0x0e];
  BattleOffset weaponOffsets[2]; /* by BattleSide.usesSecondary */
  BattleOffset base;             /* shared by all objects */
  BattleOffset unitOffsets
      [5]; /* one for each cell; some code reads base and these as one array of 6 */
  BattleOffset weaponStyleOffsets
      [2]
      [5]; /* by BattleSide.usesSecondary, then by BattleSide.armyStyle - 1 */
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
