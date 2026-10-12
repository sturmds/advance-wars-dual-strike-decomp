#include "types.h"

#define GXFIFO_COLOR ((volatile u32 *)0x04000480)
#define GXFIFO_TEXCOORD ((volatile u32 *)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32 *)0x0400048c)
#define GXFIFO_TEXIMAGE_PARAM ((volatile u32 *)0x040004a8)
#define GXFIFO_PLTT_BASE ((volatile u32 *)0x040004ac)
#define GXFIFO_BEGIN_VTXS ((volatile u32 *)0x04000500)
#define GXFIFO_END_VTXS ((volatile u32 *)0x04000504)

#define TEXCOORD(s, t) ((u16)(s) | ((u16)(t) << 16))
#define VTX_XY(x, y) ((u16)(x) | ((u16)(y) << 16))

/* Draws a w by h textured quad centred in a 64 by 128 cell at (x, y).
 * sSize and tSize are the texture size codes (TEXIMAGE_PARAM bits 20-25), shade a
 * grey level 0-31. Partial match: register allocation differs. */
void func_ov004_022ad71c(int x, int y, int z, int sSize, int tSize, int w, int h, u32 texAddr, u32 palAddr, int shade)
{
    s16 x0 = x + ((64 - w) >> 1);
    s16 y0 = y + ((128 - h) >> 1);
    u32 gray = shade | (shade << 5) | (shade << 10);
    s16 z0 = z;
    s16 y1 = y0 + h;
    s16 x1 = x + ((64 - w) >> 1) + w;   /* not x0 + w: closer to the original */

    *GXFIFO_TEXIMAGE_PARAM = (texAddr >> 3) | 0x4c000000 | (sSize << 20) | (tSize << 23) | 0x20000000;
    *GXFIFO_PLTT_BASE = palAddr >> 4;
    *GXFIFO_BEGIN_VTXS = 1;
    *GXFIFO_COLOR = (u16)gray;

    *GXFIFO_TEXCOORD = TEXCOORD(0, 0);
    *GXFIFO_VTX_16 = VTX_XY(x0, y0);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_TEXCOORD = TEXCOORD(0, 128 << 4);
    *GXFIFO_VTX_16 = VTX_XY(x0, y1);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_TEXCOORD = TEXCOORD(64 << 4, 128 << 4);
    *GXFIFO_VTX_16 = VTX_XY(x1, y1);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_TEXCOORD = TEXCOORD(64 << 4, 0);
    *GXFIFO_VTX_16 = VTX_XY(x1, y0);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_END_VTXS = 0;
}
