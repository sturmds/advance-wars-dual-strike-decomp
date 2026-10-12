#ifndef GAME_TAG_GAUGE_TASK_H
#define GAME_TAG_GAUGE_TASK_H

#include "types.h"

/* Script task of the tag firepower gauge on the tag power cut-in. A Task
 * (game/task.h) seen another way: the fields sit on Task bytes unk_4c (low
 * half), unk_52 and unk_58. */
typedef struct TagGaugeTask {
  u8 pad_00[0x4c];
  s16 frame;     /* frame counter of the running step */
  s16 popInDone; /* 1 once the pop-in step has run, 0 before */
  u8 pad_50[0x52 - 0x50];
  u16 value; /* value the gauge shows now, counts up to target */
  u8 pad_54[0x58 - 0x54];
  int target; /* tag firepower of the player's two COs, in percent */
} TagGaugeTask;

/* Position of the gauge on the main screen once it has slid in. */
#define TAG_GAUGE_X 34
#define TAG_GAUGE_Y 140

#endif
