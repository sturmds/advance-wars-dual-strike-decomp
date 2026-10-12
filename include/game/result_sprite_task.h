#ifndef GAME_RESULT_SPRITE_TASK_H
#define GAME_RESULT_SPRITE_TASK_H

#include "types.h"

/* Task of the script steps near data_0215a024 that draw the four sprites
 * of ResultPath_DrawAll and animate one more (tile 0x40a, data_027c0060) at
 * (200, 88). The first 0x2a bytes belong to the task system. */
typedef struct ResultSpriteTask {
  u8 pad_00[0x4c];
  s16 frame; /* frame of the running animation step */
} ResultSpriteTask;

#endif
