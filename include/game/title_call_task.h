#ifndef GAME_TITLE_CALL_TASK_H
#define GAME_TITLE_CALL_TASK_H

#include "types.h"

/* Counters of the title call tasks (script data_0215bbc8). The first 0x4c
 * bytes belong to the task system (Task_Create). */
typedef struct TitleCallTask {
  u8 pad_00[0x4c];
  s16 frame; /* frame counter of the running step */
  s16 hold;  /* frame counter of the step after it */
} TitleCallTask;

#endif
