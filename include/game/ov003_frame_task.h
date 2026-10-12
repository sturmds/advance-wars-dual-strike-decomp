#ifndef GAME_OV003_FRAME_TASK_H
#define GAME_OV003_FRAME_TASK_H

#include "types.h"

/* Overlay 3 task whose script counts frames in a halfword at 0x4c. */
typedef struct Ov003FrameTask {
  u8 pad_00[0x4c];
  s16 frame;
} Ov003FrameTask;

#endif
