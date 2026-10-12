#ifndef GAME_OV005_TASK_H
#define GAME_OV005_TASK_H

#include "types.h"

/* Task used by overlay 5's screens. The first 0x2c bytes belong to the task
 * system (Task_Create). Each screen's first step allocates a work block with
 * Mem_Alloc, stores it in `work` and in an overlay global
 * (data_ov005_0236bb80, data_ov005_0236bbac, ...); its last step frees it. */
typedef struct Ov005Task {
  u8 pad_00[0x2c];
  void* work;
  u32 unk_30;
  u32 step; /* state of the update function */
  u8 pad_38[0x3c - 0x38];
  int frame;
  int frames;
  int entryIndex; /* index into the entry table the task works for */
  int unk_48;
  u8 pad_4c[0x64 - 0x4c];
  struct Task* paletteTask; /* set by Ov005Screen_StartPaletteTask */
} Ov005Task;

#endif
