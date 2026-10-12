#ifndef GAME_UNK_02031A14_H
#define GAME_UNK_02031A14_H

#include "types.h"

/* Task made by func_02031a14. The first 0x2c bytes belong to the task
 * system (Task_Create). */
typedef struct Unk_02031a14 {
  u8 pad_00[0x2c];
  u32 x; /* sprite position */
  u32 y;
  u8 pad_34[0x4c - 0x34];
  s16 frame;
  u8 pad_4e[0x52 - 0x4e];
  u16 matrix; /* OBJ affine matrix index */
} Unk_02031a14;

void func_02031a14(u32 x, u32 y, int matrix);

#endif
