#include "types.h"
#include "game/Unk_02080440.h"
#include "game/camera_pan.h"
#include "game/camera_preset.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int gCameraPresetIndex;

void func_0200c500(Camera *camera)
{
    Unk_02080440_1c4 *view = data_027c027c->unk_48[data_027c0284]->unk_1c4;
    int preset = gCameraPresetIndex;
    int scrollX = view->scrollX;
    int scrollY = view->scrollY;
    s32 x = gCameraPresets[preset].x + ((scrollX << 9) >> 4);
    s32 y = gCameraPresets[preset].y - ((scrollY << 9) >> 4);
    s32 z = gCameraPresets[preset].z;

    camera->x = x - 0x100;
    camera->y = y;
    camera->z = z;
    camera->rotX = gCameraPresets[gCameraPresetIndex].rotX;
    camera->rotY = gCameraPresets[gCameraPresetIndex].rotY;
    camera->rotZ = gCameraPresets[gCameraPresetIndex].rotZ;
}
