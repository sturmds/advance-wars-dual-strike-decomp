#include "types.h"
#include "game/camera_preset.h"

extern int data_022900b4;        /* index of the preset in use */

void func_0200c7ec(void *camera, s32 x, s32 y, s32 z, u16 rotX, u16 rotY, u16 rotZ);

void Camera_ResetToPreset0(void *camera)
{
    data_022900b4 = 0;
    func_0200c7ec(camera, data_021576e8[0].x, data_021576e8[0].y, data_021576e8[0].z,
                  data_021576e8[0].rotX, data_021576e8[0].rotY, data_021576e8[0].rotZ);
}
