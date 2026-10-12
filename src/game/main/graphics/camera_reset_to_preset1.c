#include "types.h"
#include "game/camera_preset.h"

extern int gCameraPresetIndex; /* index of the preset in use */

void Camera_Set(void* camera, s32 x, s32 y, s32 z, u16 rotX, u16 rotY,
                u16 rotZ);

void Camera_ResetToPreset1(void* camera) {
  gCameraPresetIndex = 1;
  Camera_Set(camera, gCameraPresets[1].x, gCameraPresets[1].y,
             gCameraPresets[1].z, gCameraPresets[1].rotX,
             gCameraPresets[1].rotY, gCameraPresets[1].rotZ);
}
