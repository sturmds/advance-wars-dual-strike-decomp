#include "types.h"
#include "sdk/gx.h"
#include "game/camera_pan.h"
#include "game/camera_preset.h"

extern int gCameraPresetIndex;
/* gCameraPresets[0].y: the original reads the y fields through this symbol */
extern s32 data_021576ec;

int MainLoop_GetFrameCount(void);
int func_01ffd854(void); /* sets the diffuse, ambient, specular and emission colours to white */
void Gx_SendPlainQuad_020124a0(void);

/* Writes x, y, z to MTX_TRANS (0x04000470). Needed to match: the three values are loaded
 * before the first write. */
static inline void G3_Translate(fx32 x, fx32 y, fx32 z)
{
    *REG_MTX_TRANS = x;
    *REG_MTX_TRANS = y;
    *REG_MTX_TRANS = z;
}

#define GEOM_TARGET_POS_AND_DIR 2
#define GEOM_TARGET_TEXCOORD         3

/* Draws one textured quad at the camera's x and y and at z pos->z + 0x300.
 * The texture moves with the camera's offset from its preset and scrolls
 * with the frame count. */
void Map3d_DrawScrollingQuad(VecFx32 *pos, Camera *camera)
{
    /* needed to match: both preset fields are read at one byte offset */
    int offset = gCameraPresetIndex * sizeof(CameraPreset);
    s32 dx = camera->x - ((CameraPreset *)((u8 *)gCameraPresets + offset))->x - 0x200;
    s32 dy = *(s32 *)((u8 *)&data_021576ec + offset) - camera->y;
    int frame = MainLoop_GetFrameCount() & 0x7ff;

    *REG_MTX_PUSH = 0;
    G3_Translate(camera->x, camera->y, pos->z + 0x300);
    *REG_MTX_PUSH = 0;
    func_01ffd854();
    *REG_MTX_MODE = GEOM_TARGET_TEXCOORD;
    *REG_MTX_IDENTITY = 0;
    G3_Translate(((frame >> 1) + ((dx << 7) >> 9)) << 12, (((dy << 7) >> 9) - (frame >> 1)) << 12, 0);
    *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    /* A5I3 64x64 at 0x680, repeat in s and t, colour 0 transparent */
    *REG_TEXIMAGE_PARAM = 0x79b300d0;
    *REG_PLTT_BASE = 0;
    /* polygon id 2, alpha 8 of 31, front faces */
    *REG_POLYGON_ATTR = 0x02080080;
    *REG_BEGIN_VTXS = 1;
    Gx_SendPlainQuad_020124a0();
    *REG_END_VTXS = 0;
    *REG_MTX_POP = 1;
    *REG_MTX_POP = 1;
}
