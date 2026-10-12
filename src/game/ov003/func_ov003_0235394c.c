#include "types.h"

#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
#define GXFIFO_BEGIN_VTXS ((volatile u32*)0x04000500)
#define GXFIFO_END_VTXS ((volatile u32*)0x04000504)
#define TEXCOORD(s, t) ((u16)(s) | ((u16)(t) << 16))
#define VTX_XY(x, y) ((u16)(x) | ((u16)(y) << 16))

void func_ov003_0235394c(int x, int y, int z, int w, int h, int s0, int t0,
                         int s1, int t1) {
  s16 x0 = x;
  s16 y0 = y;
  s16 x1 = x + w;
  s16 y1 = y + h;
  s16 z0 = z;
  s16 u0 = s0 << 4;
  s16 v0 = t0 << 4;
  s16 u1 = s1 << 4;
  s16 v1 = t1 << 4;

  *GXFIFO_BEGIN_VTXS = 1;

  *GXFIFO_TEXCOORD = TEXCOORD(u0, v0);
  *GXFIFO_VTX_16 = VTX_XY(x0, y0);
  *GXFIFO_VTX_16 = (u16)z0;

  *GXFIFO_TEXCOORD = TEXCOORD(u0, v1);
  *GXFIFO_VTX_16 = VTX_XY(x0, y1);
  *GXFIFO_VTX_16 = (u16)z0;

  *GXFIFO_TEXCOORD = TEXCOORD(u1, v1);
  *GXFIFO_VTX_16 = VTX_XY(x1, y1);
  *GXFIFO_VTX_16 = (u16)z0;

  *GXFIFO_TEXCOORD = TEXCOORD(u1, v0);
  *GXFIFO_VTX_16 = VTX_XY(x1, y0);
  *GXFIFO_VTX_16 = (u16)z0;

  *GXFIFO_END_VTXS = 0;
}
