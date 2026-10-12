#ifndef GAME_BATTLE_CUE_TASK_H
#define GAME_BATTLE_CUE_TASK_H

#include "types.h"

/* Fields of a Task that the battle animation tasks of ov002 use to step
 * through a timed sequence (see BattleCueTask_Update). */
typedef struct BattleCueTask {
  u8 pad_00[0x4c];
  s16 frame;    /* frames since the last cue */
  s16 cueIndex; /* position in the table of cue lengths */
  s16 kind;     /* 1 to 3: which sequence to play */
  u16 unk_52;
  u8 pad_54[0x58 - 0x54];
  u32 sound; /* Sound_PlaySe id; 0xffff: none */
  u8 pad_5c[0x64 - 0x5c];
  u16 unk_64;
} BattleCueTask;

#endif
