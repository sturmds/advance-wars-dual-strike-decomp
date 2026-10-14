#ifndef GAME_CAMERA_PRESET_H
#define GAME_CAMERA_PRESET_H

#include "types.h"

/* One start position for the camera object that Camera_Set fills.
 * gCameraPresets holds an array of these. */
typedef struct CameraPreset {
  s32 x;
  s32 y;
  s32 z;
  u16 rotX;
  u16 rotY;
  u16 rotZ;
  u8 viewWidth;  /* map tiles the view shows across (MapView_FindScroll) */
  u8 viewHeight; /* and down */
} CameraPreset;

extern CameraPreset gCameraPresets[];

#endif
