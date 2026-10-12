#ifndef GAME_CURSOR_BAND_TASK_H
#define GAME_CURSOR_BAND_TASK_H

#include "types.h"

/* Task of script data_02159a3c: a row of sprites across the screen at the
 * menu cursor's height that scrolls right and pulses. The first 0x2c bytes
 * belong to the task system (Task_Create). */
typedef struct CursorBandTask {
  u8 pad_00[0x2c];
  int scrollX; /* 0-31, 2 pixels per frame */
  u8 pad_30[0x4c - 0x30];
  s16 timer; /* frames in the current step */
  u8 pad_4e[0x52 - 0x4e];
  u16 step; /* 0-5: brightening from 0 to 3, then back down */
} CursorBandTask;

#endif
