#include "types.h"

#define GXFIFO_COLOR ((volatile u32*)0x04000480)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
#define GXFIFO_POLYGON_ATTR ((volatile u32*)0x040004a4)
#define GXFIFO_TEXIMAGE_PARAM ((volatile u32*)0x040004a8)
#define GXFIFO_PLTT_BASE ((volatile u32*)0x040004ac)
#define GXFIFO_BEGIN_VTXS ((volatile u32*)0x04000500)
#define GXFIFO_END_VTXS ((volatile u32*)0x04000504)

#define TEXCOORD(s, t) ((u16)(s) | ((u16)(t) << 16))
#define VTX_XY(x, y) ((u16)(x) | ((u16)(y) << 16))

/* Like Ov004_SendTexturedQuad, for a 256-colour texture. Also sets the
 * polygon attributes: alpha (0-31) and polygon id, light 0 on, front
 * faces only. */
void Ov004_SendTexturedQuad256(int x, int y, int z, int sSize, int tSize, int w,
                               int h, int s0, int t0, u32 texAddr, u32 palAddr,
                               int alpha, int polyId) {
  s16 x0 = x;
  s16 y0 = y;
  s16 x1 = x + w;
  s16 y1 = y + h;
  s16 z0 = z;
  s16 u0 = s0 << 4;
  s16 v0 = t0 << 4;
  s16 u1 = (w + s0) << 4;
  s16 v1 = (h + t0) << 4;

  *GXFIFO_TEXIMAGE_PARAM =
      (texAddr >> 3) | 0x50000000 | (sSize << 20) | (tSize << 23) | 0x20000000;
  *GXFIFO_POLYGON_ATTR = (polyId << 24) | 0x81 | (alpha << 16);
  *GXFIFO_PLTT_BASE = palAddr >> 4;
  *GXFIFO_BEGIN_VTXS = 1;
  *GXFIFO_COLOR = 0x7fff;

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
