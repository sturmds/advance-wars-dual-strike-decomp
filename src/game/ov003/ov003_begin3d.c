#include "types.h"

#define GXFIFO_MTX_PUSH (*(volatile u32*)0x04000444)
#define GXFIFO_MTX_MODE (*(volatile u32*)0x04000440)
#define GXFIFO_MTX_IDENTITY (*(volatile u32*)0x04000454)
#define GXFIFO_TEXIMAGE_PARAM (*(volatile u32*)0x04000484)
#define GXFIFO_POLYGON_ATTR (*(volatile u32*)0x040004a4)
#define GEOM_TARGET_TEXCOORD 3
#define GEOM_TARGET_POS_AND_DIR 2
extern u8 data_ov003_02364a78[];
extern u8 data_ov003_02364a6c[];
static u8 data_ov003_02365cf4[0xc]; /* size: up to the next symbol */
u8 data_ov003_02365d00[0x10];       /* size: up to the next symbol */
/* per tile code: bits 5-7 row, bits 0-4 terrain; size: up to the next symbol */
u8 data_ov003_02365d10[0x400];
u16 data_ov003_02367110[512][4]; /* layer 2 and 3 map entries per 16x16 tile */
u16 data_ov003_02366110[512][4]; /* layer 1 map entries per 16x16 tile */
u8 data_ov003_02368110[0x1424];  /* size: up to the next symbol */
void G3X_Reset(void);
void G3i_LookAt_(void* a, void* b, void* c, int d, int e);
void func_ov003_02353b34(void);

/* Per-frame 3D setup of overlay 3 (Ov003_DrawFrame): pushes a matrix, resets
 * the texture matrix, sets the default texture and polygon attributes. */
void Ov003_Begin3d(void) {
  G3X_Reset();
  G3i_LookAt_(data_ov003_02364a78, data_ov003_02364a6c, data_ov003_02365cf4, 1,
              0);
  func_ov003_02353b34();
  GXFIFO_MTX_PUSH = 0;
  GXFIFO_MTX_MODE = GEOM_TARGET_TEXCOORD;
  GXFIFO_MTX_IDENTITY = 0;
  GXFIFO_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
  GXFIFO_TEXIMAGE_PARAM = 0x20000000;
  GXFIFO_POLYGON_ATTR = 0x1f0081;
}
