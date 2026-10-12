#ifndef GAME_BRIGHTNESS_FADE_H
#define GAME_BRIGHTNESS_FADE_H

#include "types.h"
#include "game/task.h"

/* Fields of the fade task BrightnessFade_Start creates. */
typedef struct BrightnessFadeTask {
  u8 pad_00[0x30];
  int active; /* 1 while the fade runs, 0 once done */
  int step;   /* 0 fading, 1 one more frame, 2 done */
  u8 pad_38[0x3c - 0x38];
  int frame;  /* starts at 0 */
  int frames; /* length of the fade */
  u8 pad_44[0x48 - 0x44];
  int screen; /* 0 or 1, index into data_027c0288 */
  u8 pad_4c[0x54 - 0x4c];
  int startLevel; /* master brightness, -16 to 16 */
  int endLevel;
} BrightnessFadeTask;

/* BrightnessFade_Start starts a task that fades one screen's brightness over
 * `frames` frames and returns it. `out` nonzero fades from 0 to the target
 * level, zero from the target level back to 0. `screen` picks the screen
 * (0 or 1 in data_027c0288). `white` nonzero makes the level +16 (white, a guess), zero -16.
 * The wrappers below fix the three flags; parentOrList is Task_Create's
 * second argument (a parent task, or TASK_LIST(n)). */
Task* BrightnessFade_Start(void* parentOrList, int out, int screen, int white,
                           int frames);

Task* func_020fccd8(void* parentOrList, int frames); /* out, screen 0, white */
Task* BrightnessFade_StartInWhiteScreen1(void* parentOrList,
                                         int frames); /* in, screen 1, white */
Task* func_020fcd28(void* parentOrList, int frames);  /* in, screen 0, white */
Task* func_020fcd50(void* parentOrList, int frames);  /* out, screen 1, black */
Task* BrightnessFade_StartOutBlackScreen0(
    void* parentOrList, int frames);                 /* out, screen 0, black */
Task* func_020fcda0(void* parentOrList, int frames); /* in, screen 1, black */
Task* func_020fcdc8(void* parentOrList, int frames); /* in, screen 0, black */

#endif
