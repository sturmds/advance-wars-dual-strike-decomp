#ifndef GAME_CO_EXP_H
#define GAME_CO_EXP_H

#include "types.h"

/* Experience of one CO on the results screen, a guess from how
 * CoExp_TallyStep fills it: 1000 points make a level, up to level 100. */
typedef struct CoExp {
  s8 level; /* below 0: no CO in this slot */
  u8 levelUpFrame; /* 0; 1 when a point gain raises the level, then counts frames up to 255 */
  s16 points; /* 0-999 toward the next level */
} CoExp;

#define CO_EXP_PER_LEVEL 1000
#define CO_EXP_MAX_LEVEL 100

/* Results screen layout of MenuState. */
typedef struct CoExpScreenWork {
  u8 pad_020[0x1364 - 0x20];
  u8 unk_1364
      [0x160]; /* sprite file (localized), passed to SpriteAnimTask_Start by the CO cell tasks */
  u8 cellSprites[0xe4]; /* sprite file of CoExp_AddCellSprite */
  CoExp exp[6];         /* two per player: front and back CO */
} CoExpScreenWork;

/* Task started by CoExp_StartLevelUpTask, 0x6c bytes like Task. */
typedef struct CoExpLevelUpTask {
  u8 pad_00[0x4e];
  s16 count; /* COs that gained a level */
  u8 pad_50[0x52 - 0x50];
  u16 mask; /* bit per CO slot of those COs */
} CoExpLevelUpTask;

#endif
