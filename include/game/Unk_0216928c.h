#ifndef GAME_UNK_0216928C_H
#define GAME_UNK_0216928C_H

#include "types.h"

/* Task made from data_0216928c by func_020eee1c. The first 0x2c bytes belong
 * to the task system (Task_Create). */
typedef struct Unk_0216928c {
  u8 pad_00[0x2c];
  int unk_2c;
  int unk_30;
  u8 pad_34[0x4a - 0x34];
  u16 unk_4a;
  u8 pad_4c[0x54 - 0x4c];
  int unk_54;
} Unk_0216928c;

#endif
