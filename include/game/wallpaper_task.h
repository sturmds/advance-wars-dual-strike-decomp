#ifndef GAME_WALLPAPER_TASK_H
#define GAME_WALLPAPER_TASK_H

#include "types.h"
#include "game/co_data.h"

/* Wallpaper ids: 0 is the default, 1 to CO_COUNT - 1 the wallpaper of that
 * CO id, then the extra wallpapers that game flags 64, 65 and 63 make
 * available in the wallpaper list (Ov005_AddCoListButtons). */
#define WALLPAPER_EXTRA_FIRST CO_COUNT
#define WALLPAPER_EXTRA_COUNT 3
#define WALLPAPER_COUNT (WALLPAPER_EXTRA_FIRST + WALLPAPER_EXTRA_COUNT)

/* Bytes of wallpaper tile data the sub screen copies from the main screen. */
#define WALLPAPER_TILES_SIZE 0x10000

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

/* per wallpaper: offset of its tiles in the file */
extern u16 data_0229c684[];

#endif
