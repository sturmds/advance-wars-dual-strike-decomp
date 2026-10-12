#include "types.h"
#include "game/camera_preset.h"

extern int gCameraPresetIndex; /* index of the preset in use */

void Camera_Set(void* camera, s32 x, s32 y, s32 z, u16 rotX, u16 rotY,
                u16 rotZ);

void Camera_ResetToPreset0(void* camera) {
  gCameraPresetIndex = 0;
  Camera_Set(camera, gCameraPresets[0].x, gCameraPresets[0].y,
             gCameraPresets[0].z, gCameraPresets[0].rotX,
             gCameraPresets[0].rotY, gCameraPresets[0].rotZ);
}
