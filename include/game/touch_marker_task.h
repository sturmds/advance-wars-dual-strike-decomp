#ifndef GAME_TOUCH_MARKER_TASK_H
#define GAME_TOUCH_MARKER_TASK_H

#include "types.h"

/* Task that TouchMarker_Create starts on the tile where a touch begins. The
 * first 0x2c bytes belong to the task system (Task_Create). */
typedef struct TouchMarkerTask {
    u8 pad_00[0x2c];
    int tileX;
    int tileY;
    u8 pad_34[0x44 - 0x34];
    s16 age;                           /* frames, counts up to TOUCH_MARKER_FRAMES */
} TouchMarkerTask;

#define TOUCH_MARKER_FRAMES 8

#endif
