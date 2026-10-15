#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"

#define GXFIFO_MTX_MODE ((volatile u32*)0x04000440)
#define GXFIFO_MTX_PUSH ((volatile u32*)0x04000444)
#define GXFIFO_MTX_IDENTITY ((volatile u32*)0x04000454)
#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_POLYGON_ATTR ((volatile u32*)0x040004a4)
#define GXFIFO_DIF_AMB ((volatile u32*)0x040004c0)
#define GXFIFO_SPE_EMI ((volatile u32*)0x040004c4)
#define GXFIFO_LIGHT_VECTOR ((volatile u32*)0x040004c8)
#define GXFIFO_LIGHT_COLOR ((volatile u32*)0x040004cc)
void G3i_LookAt_(const void* a, const void* b, void* c, int d, int e);
extern u8 data_ov005_023695d0[];
extern u8 data_ov005_023695c4[];
/* The look-at target; never written, so the origin. */
static VecFx32 data_ov005_0236bb94;

void func_ov005_0235b2f0(void) {
  G3X_Reset();
  G3i_LookAt_(data_ov005_023695d0, data_ov005_023695c4, &data_ov005_0236bb94, 1,
              0);
  *GXFIFO_LIGHT_VECTOR = 0x1ff00000;
  *GXFIFO_LIGHT_COLOR = 0x7fff;
  *GXFIFO_DIF_AMB = 0x7fffffff;
  *GXFIFO_SPE_EMI = 0x4210;
  *GXFIFO_MTX_PUSH = 0;
  *GXFIFO_MTX_MODE = 3;
  *GXFIFO_MTX_IDENTITY = 0;
  *GXFIFO_MTX_MODE = 2;
  *GXFIFO_NORMAL = 0x20000000;
  *GXFIFO_POLYGON_ATTR = 0x001f0081;
}
