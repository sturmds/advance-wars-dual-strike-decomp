#ifndef GAME_OV003_MESSAGE_TASK_H
#define GAME_OV003_MESSAGE_TASK_H

#include "types.h"

/* Task of overlay 3 that shows a text message for a fixed number of frames,
 * then resumes its script (Ov003_ShowTwoLineMessage, Ov003_ShowWarpedMessage). */
typedef struct Ov003MessageTask {
  u8 pad_00[0x4c];
  s16 frames; /* frames shown, counted while the byte at 0x1d7 of the state is 0 */
} Ov003MessageTask;

#endif
