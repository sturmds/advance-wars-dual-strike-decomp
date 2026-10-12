#ifndef GAME_WALLPAPER_TASK_H
#define GAME_WALLPAPER_TASK_H

#include "types.h"

/* The task of script gWallpaperTaskScript, which Wallpaper_LoadSelected starts on
 * each screen: it scrolls the wallpaper layers and blends between them.
 * The first 0x2a bytes belong to the task system (Task_Create). */
typedef struct WallpaperTask {
  u8 pad_00[0x4c];
  s16 frame; /* frames since the start */
  u8 pad_4e[0x64 - 0x4e];
  s16 scroll2; /* layer 2 x scroll, one step every 8 frames */
  s16 scroll3; /* layer 3 x scroll, the other way */
  s16 blendStep; /* one step every 4 frames; bits 4-6 pick the phase, bits 0-3 the level */
} WallpaperTask;

/* The task of script data_0215937c, which the single-wallpaper loaders
 * start: it scrolls layer 3 vertically. */
typedef struct WallpaperSingleTask {
  u8 pad_00[0x4c];
  s16 frame; /* frames since the start */
  u8 pad_4e[0x64 - 0x4e];
  s16 scrollY; /* layer 3 y scroll, one step every 2 frames, 0 to 320 */
} WallpaperSingleTask;

#endif
