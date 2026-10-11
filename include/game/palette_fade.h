#ifndef GAME_PALETTE_FADE_H
#define GAME_PALETTE_FADE_H

#include "types.h"
#include "game/task.h"

/* Fields of the task PaletteFade_Start creates: it darkens a run of 16-color
 * BG palettes of one screen from one level to another (0 none, 16 black). */
typedef struct PaletteFadeTask {
    u8 pad_00[0x30];
    int active;                        /* 1 while the fade runs, 0 once done */
    int step;                          /* 0-1 fading, 2 frees colors, 3 done */
    u8 pad_38[0x3c - 0x38];
    int frame;                         /* starts at 0 */
    int frames;                        /* length of the fade */
    int firstPalette;
    int paletteCount;
    int startLevel;
    int endLevel;
    int screen;                        /* 0 or 1, index into data_027c0288 */
    u8 pad_58[0x64 - 0x58];
    u8 *palettes;                      /* first faded palette in the screen's paletteBuffer */
    u8 *colors;                        /* undarkened colors, paletteCount * 32 bytes */
} PaletteFadeTask;

#endif
