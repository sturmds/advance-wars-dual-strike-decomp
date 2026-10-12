#include "types.h"

#define GXFIFO_BEGIN_VTXS ((volatile u32*)0x04000500)
#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
#define GXFIFO_END_VTXS ((volatile u32*)0x04000504)

/* Sends a textured quad in the XZ plane at height y to the 3D geometry FIFO, facing -Y. It
 * covers width in x and depth in z. u0 v0 and u1 v1 are the texture corners in texels. Needed to
 * match: the s16 locals, which the compiler narrows one by one. */
void Gx_SendQuadXZ(int x, int y, int z, int width, int depth, int u0, int v0,
                   int u1, int v1) {
  s16 x1 = x;
  s16 y1 = y;
  s16 z1 = z;
  s16 x2 = x + width;
  s16 z2 = z + depth;
  s16 s0 = u0 << 4;
  s16 t0 = v0 << 4;
  s16 s1 = u1 << 4;
  s16 t1 = v1 << 4;

  *GXFIFO_BEGIN_VTXS = 1;
  *GXFIFO_NORMAL = 0x80000;
  *GXFIFO_TEXCOORD = (u16)s0 | ((u16)t0 << 16);
  *GXFIFO_VTX_16 = (u16)x1 | ((u16)y1 << 16);
  *GXFIFO_VTX_16 = (u16)z2;
  *GXFIFO_TEXCOORD = (u16)s0 | ((u16)t1 << 16);
  *GXFIFO_VTX_16 = (u16)x1 | ((u16)y1 << 16);
  *GXFIFO_VTX_16 = (u16)z1;
  *GXFIFO_TEXCOORD = (u16)s1 | ((u16)t1 << 16);
  *GXFIFO_VTX_16 = (u16)x2 | ((u16)y1 << 16);
  *GXFIFO_VTX_16 = (u16)z1;
  *GXFIFO_TEXCOORD = (u16)s1 | ((u16)t0 << 16);
  *GXFIFO_VTX_16 = (u16)x2 | ((u16)y1 << 16);
  *GXFIFO_VTX_16 = (u16)z2;
  *GXFIFO_END_VTXS = 0;
}
