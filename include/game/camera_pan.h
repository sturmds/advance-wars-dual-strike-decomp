#ifndef GAME_CAMERA_PAN_H
#define GAME_CAMERA_PAN_H

#include "types.h"

/* The camera object that Camera_Set fills (data_022a7d5c). Only the rotation
 * and position are known. */
typedef struct Camera {
    u16 rotX;
    u16 rotY;
    u16 rotZ;
    u8 pad_06[0x02];
    s32 x;
    s32 y;
    s32 z;
} Camera;

/* Fields at the end of a Task that eases the camera between two world
 * positions over `frames` frames. Overlays the Task fields from 0x34. */
typedef struct CameraPanTask {
    u8 pad_00[0x34];
    s32 from[3];                       /* x, y, z */
    s32 to[3];
    u8 pad_4c[0x64 - 0x4c];
    s16 frames;                        /* length of the pan */
    s16 frame;                         /* frames done so far */
} CameraPanTask;

#endif
