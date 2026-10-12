#include "types.h"

#define GXFIFO_TEXIMAGE_PARAM ((volatile u32*)0x040004a8)
#define GXFIFO_PLTT_BASE ((volatile u32*)0x040004ac)

u32 Battle_GetTexHeight(u32 index);
u32 Battle_GetTexWidth(u32 index);
void Gx_SendQuadXY(int x, int y, int z, int width, int height, int u0, int v0,
                   int u1, int v1);

/* Packs and writes the TEXIMAGE_PARAM register (0x040004a8). Needed to match: as an inline function its
 * arguments get their own registers, which a plain expression does not. */
static inline void G3_TexImageParam(int texFmt, int texGen, int s, int t,
                                    int repeat, int flip, int pltt0, u32 addr) {
  *GXFIFO_TEXIMAGE_PARAM =
      (u32)((addr >> 3) | (texFmt << 26) | (texGen << 30) | (s << 20) |
            (t << 23) | (repeat << 16) | (flip << 18) | (pltt0 << 29));
}

/* Sets the texture and palette, then draws the quad with the texture flipped as asked. The
 * texture sizes are texel counts picked by sizeS and sizeT (0-3: 8 to 64). */
void Battle_DrawTexturedQuad(int x, int y, int z, int width, int height,
                             u32 sizeS, u32 sizeT, u32 texAddr, u32 palAddr,
                             int flipX, int flipY) {
  u32 texW = Battle_GetTexWidth(sizeS);
  u32 texH = Battle_GetTexHeight(sizeT);

  /* 16 colours, texture coordinates as given, no repeat or flip, colour 0 transparent.
     * texAddr and palAddr count 32-byte blocks. */
  G3_TexImageParam(3, 1, sizeS, sizeT, 0, 0, 1, texAddr << 5);
  *GXFIFO_PLTT_BASE = (palAddr << 5) >> 4;
  if (flipX != 0 && flipY != 0) {
    Gx_SendQuadXY(x, y, z, width, height, texW, texH, 0, 0);
  } else if (flipX != 0) {
    Gx_SendQuadXY(x, y, z, width, height, texW, 0, 0, texH);
  } else if (flipY != 0) {
    Gx_SendQuadXY(x, y, z, width, height, 0, texH, texW, 0);
  } else {
    Gx_SendQuadXY(x, y, z, width, height, 0, 0, texW, texH);
  }
}
