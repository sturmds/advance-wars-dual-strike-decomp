#ifndef GAME_UNK_OV005_02351DD4_H
#define GAME_UNK_OV005_02351DD4_H

#include "types.h"
#include "game/task.h"

/* Work block of an overlay 5 screen that waits for a child task
 * (Ov005Task.work). Only the child task pointer is known. */
typedef struct Unk_ov005_02351dd4 {
  u8 pad_00[0x20];
  Task* waitTask; /* done when BrightnessFade_IsActive returns 0 */
} Unk_ov005_02351dd4;

#endif
