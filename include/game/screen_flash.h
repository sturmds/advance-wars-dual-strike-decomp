#ifndef GAME_SCREEN_FLASH_H
#define GAME_SCREEN_FLASH_H

#include "types.h"
#include "game/task.h"

/* Fields of the task ScreenFlash_Start creates: the current screen's master
 * brightness fades from 0 up to 16 (white), holds, then fades back to 0. A
 * phase with 0 frames is skipped. */
typedef struct ScreenFlashTask {
  u8 pad_00[0x54];
  int fadeInFrame; /* frames done in the fade up */
  int holdFrame;
  int fadeOutFrame;
  u8 pad_60[0x64 - 0x60];
  s16 fadeInFrames;
  s16 holdFrames;
  s16 fadeOutFrames;
} ScreenFlashTask;

Task* ScreenFlash_Start(int fadeInFrames, int holdFrames, int fadeOutFrames,
                        Task* parent);

#endif
