#include "types.h"
#include "game/camera_preset.h"

extern int gCameraPresetIndex;

/* World position of a map tile: the preset's position plus 0x200 per tile,
 * y running the other way. */
void Camera_TileToWorld(s32* out, int tileX, int tileY) {
  out[0] = gCameraPresets[gCameraPresetIndex].x + (tileX << 9) - 0x1000;
  out[1] = gCameraPresets[gCameraPresetIndex].y - (tileY << 9) + 0xf00;
  out[2] = 0;
}
