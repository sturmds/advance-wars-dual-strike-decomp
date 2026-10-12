#ifndef GAME_POINTS_TALLY_TASK_H
#define GAME_POINTS_TALLY_TASK_H

#include "types.h"

#define POINTS_TALLY_MAX 9999 /* the total stops here */

/* Fields of the tasks of script data_0215b164, which show two numbers in
 * a two-row sprite box and count the first one into the second. The
 * first 0x34 bytes belong to the task system (Task_Create). */
typedef struct PointsTallyTask {
  u8 pad_00[0x34];
  int added; /* points still to move into total */
  int total;
  u8 pad_3c[0x44 - 0x3c];
  s16 frame;
  u8 pad_46[0x48 - 0x46];
  u8 frames; /* PointsTally_PopIn resumes the script when frame reaches it */
} PointsTallyTask;

#endif
