#ifndef GAME_CAMERA_PAN_H
#define GAME_CAMERA_PAN_H

#include "types.h"
#include "sdk/fx.h"

/* The camera object that Camera_Set fills (data_022a7d5c). Rotation, position, matrices
 * and look-at vectors are known. */
typedef struct Camera {
  u16 rotX;
  u16 rotY;
  u16 rotZ;
  u8 pad_06[0x02];
  s32 x;
  s32 y;
  s32 z;
  MtxFx43 mtx43;  /* 0x14 */
  MtxFx33 invRot; /* 0x44: inverse of mtx43's 3x3 part */
  MtxFx44 mtx44;  /* 0x68 */
  MtxFx33 mtx33;  /* 0xa8 */
  u8 pad_cc[0xe8 - 0xcc];
  VecFx32 eye;    /* Camera_Set starts it at (0, 0, 3.0) */
  VecFx32 target; /* starts at the origin */
  VecFx32 up;     /* starts at (0, 1.0, 0) */
} Camera;

/* Fields at the end of a Task that eases the camera between two world
 * positions over `frames` frames. Overlays the Task fields from 0x34. */
typedef struct CameraPanTask {
  u8 pad_00[0x34];
  s32 from[3]; /* x, y, z */
  s32 to[3];
  u8 pad_4c[0x64 - 0x4c];
  s16 frames; /* length of the pan */
  s16 frame;  /* frames done so far */
} CameraPanTask;

#endif
