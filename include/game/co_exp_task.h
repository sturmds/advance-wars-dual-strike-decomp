#ifndef GAME_CO_EXP_TASK_H
#define GAME_CO_EXP_TASK_H

#include "types.h"

/* Task of the CO experience page of the results screen. The first 0x2c
 * bytes belong to the task system. */
typedef struct CoExpTask {
    u8 pad_00[0x2c];
    s32 x;                             /* screen position of the sprite the task draws */
    s32 y;
    u8 pad_34[0x4c - 0x34];
    s16 frame;                         /* frame of the running animation step */
} CoExpTask;

#endif
