#ifndef GAME_WINDOW_WIPE_H
#define GAME_WINDOW_WIPE_H

#include "types.h"

/* Screen wipes drawn with window 0: an H-blank DMA task copies one WIN0H
 * value per scanline from data_0216e338 into the register. */

#define WIPE_LINES 192

/* Which screens a wipe runs on (func_020076f8): */
#define WIPE_SCREEN_CURRENT 0
#define WIPE_SCREEN_BOTH    1
#define WIPE_SCREEN_MAIN    2
#define WIPE_SCREEN_SUB     3

extern u16 data_0216e338[WIPE_LINES]; /* WIN0H per scanline: left << 8 | right */
extern s16 data_0216e29c;             /* wipe progress in pixels */
extern s16 data_0216e2a8;

int func_020076f8(void);

#endif
