#include "game/screen_wipe.h"
#ifndef GAME_WINDOW_WIPE_H
#define GAME_WINDOW_WIPE_H

#include "types.h"
#include "game/screen_effect.h"

/* Screen wipes drawn with window 0: an H-blank DMA task copies one WIN0H
 * value per scanline from gWipeWin0hTable into the register. */

#define WIPE_LINES 192

extern u16
    gWipeWin0hTable[WIPE_LINES]; /* WIN0H per scanline: left << 8 | right */

#endif
