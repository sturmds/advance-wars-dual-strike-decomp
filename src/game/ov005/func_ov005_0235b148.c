#include "types.h"

#define GXFIFO_TEXCOORD ((volatile u32 *)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32 *)0x0400048c)
#define GXFIFO_TEXIMAGE_PARAM ((volatile u32 *)0x040004a8)
#define GXFIFO_PLTT_BASE ((volatile u32 *)0x040004ac)
#define GXFIFO_BEGIN_VTXS ((volatile u32 *)0x04000500)
#define GXFIFO_END_VTXS ((volatile u32 *)0x04000504)

void func_ov005_0235b148(int unused, int x, int y, int z, int sizeS, int sizeT, int width,
                         int height, int texS, int texT, u32 texAddr, u32 palAddr)
{
    /* Texture coordinates are 12.4 fixed point. */
    s16 t0 = texT << 4;
    s16 s0 = texS << 4;
    s16 y0 = y;
    s16 x0 = x;
    s16 t1 = (height + texT) << 4;
    s16 z0 = z;
    s16 y1 = y + height;
    s16 s1 = (width + texS) << 4;
    s16 x1 = x + width;

    *GXFIFO_TEXIMAGE_PARAM = (texAddr >> 3) | 0x4c000000 | (sizeS << 20) | (sizeT << 23) | 0x20000000;
    *GXFIFO_PLTT_BASE = palAddr >> 4;
    *GXFIFO_BEGIN_VTXS = 1;

    *GXFIFO_TEXCOORD = (u16)s0 | ((u16)t0 << 16);
    *GXFIFO_VTX_16 = (u16)x0 | ((u16)y0 << 16);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_TEXCOORD = (u16)s0 | ((u16)t1 << 16);
    *GXFIFO_VTX_16 = (u16)x0 | ((u16)y1 << 16);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_TEXCOORD = (u16)s1 | ((u16)t1 << 16);
    *GXFIFO_VTX_16 = (u16)x1 | ((u16)y1 << 16);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_TEXCOORD = (u16)s1 | ((u16)t0 << 16);
    *GXFIFO_VTX_16 = (u16)x1 | ((u16)y0 << 16);
    *GXFIFO_VTX_16 = (u16)z0;

    *GXFIFO_END_VTXS = 0;
}
