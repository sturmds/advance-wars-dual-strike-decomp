#include "types.h"
#include "sdk/fx.h"
#include "game/Ov003Fx.h"
#include "game/ov003_gfx.h"
#include "game/ov003_unit.h"

#define GXFIFO_MTX_PUSH (*(volatile u32*)0x04000444)
#define GXFIFO_MTX_MODE (*(volatile u32*)0x04000440)
#define GXFIFO_MTX_IDENTITY (*(volatile u32*)0x04000454)
#define GXFIFO_TEXIMAGE_PARAM (*(volatile u32*)0x04000484)
#define GXFIFO_POLYGON_ATTR (*(volatile u32*)0x040004a4)
#define GEOM_TARGET_TEXCOORD 3
#define GEOM_TARGET_POS_AND_DIR 2
static u8 data_ov003_02364a6c[] = {
#include "data/ov003/data_ov003_02364a6c.inc.c"
};
static u8 data_ov003_02364a78[] = {
#include "data/ov003/data_ov003_02364a78.inc.c"
};
/* The look-at target; never written, so the origin. */
static VecFx32 data_ov003_02365cf4;
Ov003Fx data_ov003_02365d00;
u8 data_ov003_02365d10[OV003_TILE_CODE_COUNT];
u16 data_ov003_02367110[OV003_METATILE_COUNT][4];
u16 data_ov003_02366110[OV003_METATILE_COUNT][4];
Ov003Map data_ov003_02368110;
void G3X_Reset(void);
void G3i_LookAt_(void* a, void* b, void* c, int d, int e);
void func_ov003_02353b34(void);

/* Per-frame 3D setup of overlay 3 (Ov003_DrawFrame): pushes a matrix, resets
 * the texture matrix, sets the default texture and polygon attributes. */
void Ov003_Begin3d(void) {
  G3X_Reset();
  G3i_LookAt_(data_ov003_02364a78, data_ov003_02364a6c, &data_ov003_02365cf4, 1,
              0);
  func_ov003_02353b34();
  GXFIFO_MTX_PUSH = 0;
  GXFIFO_MTX_MODE = GEOM_TARGET_TEXCOORD;
  GXFIFO_MTX_IDENTITY = 0;
  GXFIFO_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
  GXFIFO_TEXIMAGE_PARAM = 0x20000000;
  GXFIFO_POLYGON_ATTR = 0x1f0081;
}
