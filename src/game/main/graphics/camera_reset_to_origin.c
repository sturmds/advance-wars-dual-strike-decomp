#include "types.h"
#include "game/camera_pan.h"

extern Camera data_0228ffa8;

void Camera_Set(void* camera, s32 x, s32 y, s32 z, u16 rotX, u16 rotY,
                u16 rotZ);

/* Puts the camera at data_0228ffa8 back at the origin, no rotation. */
void Camera_ResetToOrigin(void) {
  Camera_Set(&data_0228ffa8, 0, 0, 0, 0, 0, 0);
}
