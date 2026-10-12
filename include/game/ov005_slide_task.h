#ifndef GAME_OV005_SLIDE_TASK_H
#define GAME_OV005_SLIDE_TASK_H

#include "types.h"
#include "game/Unk_ov005_0236bb90.h"

/* Task that eases Ov005SlideEntry.pos from `from` to `to` over `frames`
 * frames (Ov005Slide_Update). The first 0x2c bytes belong to the task system. */
typedef struct Ov005SlideTask {
  u8 pad_00[0x2c];
  Unk_ov005_0236bb90* work;
  int busy; /* 1 while sliding; BrightnessFade_IsActive reads it */
  u32 step;
  u8 pad_38[0x3c - 0x38];
  int frame;
  int frames;
  int index;        /* entry in `entries` */
  int resumeScript; /* nonzero: resume the parent script when done */
  u8 pad_4c[0x54 - 0x4c];
  int from;
  int to;
  u8 pad_5c[0x64 - 0x5c];
  Ov005SlideEntry* entries;
} Ov005SlideTask;

Task* Ov005Slide_StartEntryA(Unk_ov005_0236bb90* work, int index);
Task* Ov005Slide_StartEntryB(Unk_ov005_0236bb90* work, int index);

#endif
