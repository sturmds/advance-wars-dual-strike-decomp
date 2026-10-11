#ifndef GAME_DAY_BANNER_TASK_H
#define GAME_DAY_BANNER_TASK_H

#include "types.h"

/* Task of the day banner script (gDayBannerScript): three word sprites and the
 * day number, one sprite per digit. DayBanner_Init fills it from the current
 * day. The first 0x2c bytes belong to the task system. */
typedef struct DayBannerTask {
    u8 pad_00[0x34];
    int digitsX;                       /* x of the first digit sprite; digits are 28 pixels apart */
    u8 pad_38[0x3c - 0x38];
    int wordX;                         /* x of the first word sprite; they are 32 pixels apart */
    u8 pad_40[0x4c - 0x40];
    s16 frame;
    u8 pad_4e[0x58 - 0x4e];
    int digits;                        /* one decimal digit per nibble, the leftmost digit in bits 0-3 */
    u8 pad_5c[0x64 - 0x5c];
    s16 digitCount;                    /* 1 to 3 */
} DayBannerTask;

#endif
