#ifndef GAME_LINK_MARKER_TASK_H
#define GAME_LINK_MARKER_TASK_H

#include "types.h"

/* Task of a marker next to a button of the two-player link menu
 * (LinkMenu_CreateMarker). The first 0x2c bytes belong to the task system. */
typedef struct LinkMarkerTask {
    u8 pad_00[0x2c];
    int x;                             /* screen position of the marker */
    int y;
    u8 pad_34[0x4c - 0x34];
    s16 frame;                         /* frames of the grow-in so far */
    u8 pad_4e[0x52 - 0x4e];
    u16 affineIndex;                   /* OAM affine parameter set, 25 + kind */
} LinkMarkerTask;

#endif
