#ifndef GAME_WINDOW_WIPE_H
#define GAME_WINDOW_WIPE_H

#include "types.h"
#include "game/screen_effect.h"

/* Screen wipes drawn with window 0: an H-blank DMA task copies one WIN0H
 * value per scanline from gWipeWin0hTable into the register. */

#define WIPE_LINES 192

extern u16
    gWipeWin0hTable[WIPE_LINES]; /* WIN0H per scanline: left << 8 | right */
extern s16 data_0216e29c;        /* wipe progress in pixels */
extern s16 data_0216e2a8;

#endif
