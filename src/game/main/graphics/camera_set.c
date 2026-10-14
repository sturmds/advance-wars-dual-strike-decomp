#include "types.h"
#include "sdk/fx.h"
#include "game/camera_pan.h"
#include "game/camera_preset.h"

static VecFx32 data_021576dc = {
#include "data/main/data_021576dc.inc.c"
};
static VecFx32 data_021576d0 = {
#include "data/main/data_021576d0.inc.c"
};
CameraPreset gCameraPresets[5] = {
#include "data/main/gCameraPresets.inc.c"
};

/* Places the camera at (x, y, z) with the given rotation and resets its
 * look-at vectors to their defaults. */
void Camera_Set(Camera* camera, s32 x, s32 y, s32 z, u16 rotX, u16 rotY,
                u16 rotZ) {
  VecFx32 eye = data_021576dc;
  VecFx32 target = {0, 0, 0};
  VecFx32 up = data_021576d0;

  camera->eye = eye;
  camera->target = target;
  camera->up = up;
  camera->x = x;
  camera->y = y;
  camera->z = z;
  camera->rotX = rotX;
  camera->rotY = rotY;
  camera->rotZ = rotZ;
}
