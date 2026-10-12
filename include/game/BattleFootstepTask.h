#ifndef GAME_BATTLEFOOTSTEPTASK_H
#define GAME_BATTLEFOOTSTEPTASK_H

#include "types.h"

/* Sound task of a foot unit in the ov002 battle animation, made by
 * BattleFootstepTask_Start. The first 0x2c bytes belong to the task system. */
typedef struct BattleFootstepTask {
  u8 pad_00[0x4c];
  u16 unk_4c; /* cleared by Start */
  u16 unk_4e; /* cleared by Start */
  u8 pad_50[2];
  u16 side; /* 0 or 1 */
  u8 pad_54[4];
  u32 sound; /* sub sound effect id: group in the high half, variant in the low half; picked by terrain */
} BattleFootstepTask;

#endif
