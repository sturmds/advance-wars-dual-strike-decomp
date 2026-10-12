#include "types.h"

#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
#define GXFIFO_TEXIMAGE_PARAM ((volatile u32*)0x040004a8)
#define GXFIFO_PLTT_BASE ((volatile u32*)0x040004ac)
#define GXFIFO_BEGIN_VTXS ((volatile u32*)0x04000500)
#define GXFIFO_END_VTXS ((volatile u32*)0x04000504)

/* Packs and writes the TEXIMAGE_PARAM register (0x040004a8). Needed to match: as an inline function its
 * arguments get their own registers, which a plain expression does not. */
static inline void G3_TexImageParam(int texFmt, int texGen, int s, int t,
                                    int repeat, int flip, int pltt0, u32 addr) {
  *GXFIFO_TEXIMAGE_PARAM =
      (u32)((addr >> 3) | (texFmt << 26) | (texGen << 30) | (s << 20) |
            (t << 23) | (repeat << 16) | (flip << 18) | (pltt0 << 29));
}

/* Sends one quad at (x, y), size width by height, showing the same-sized part
 * of a 16-colour texture that starts at (texS, texT). sizeS and sizeT are the
 * texture size codes (TEXIMAGE_PARAM bits 20-25). */
void Ov005_SendTexturedQuad(int unused, int x, int y, int z, int sizeS,
                            int sizeT, int width, int height, int texS,
                            int texT, u32 texAddr, u32 palAddr) {
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

  /* 16 colours, texture coordinates as given, no repeat or flip, colour 0 transparent. */
  G3_TexImageParam(3, 1, sizeS, sizeT, 0, 0, 1, texAddr);
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
