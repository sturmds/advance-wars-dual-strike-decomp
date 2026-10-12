#ifndef GAME_OV005_ENTRY_TASK_H
#define GAME_OV005_ENTRY_TASK_H

#include "types.h"
#include "game/Unk_ov005_0236bb90.h"

/* Task that animates one entry of Unk_ov005_0236bb90.entriesA. The first 0x2c
 * bytes belong to the task system. */
typedef struct Ov005EntryTask {
  u8 pad_00[0x2c];
  Unk_ov005_0236bb90* work;
  int busy; /* 1 while the animation runs */
  u32 step;
  u8 pad_38[0x3c - 0x38];
  int frame;
  int frames;
  int index;       /* entry in work->entriesA */
  int firstFrames; /* duration of the first phase */
} Ov005EntryTask;

#endif
