#include "types.h"

#define TEXIMAGE_PARAM ((volatile u32*)0x040004a8)
#define PLTT_BASE ((volatile u32*)0x040004ac)

/* Packs and writes the TEXIMAGE_PARAM register (0x040004a8). Needed to match: as an inline function its
 * arguments get their own registers, which a plain expression does not. */
static inline void G3_TexImageParam(int texFmt, int texGen, int s, int t,
                                    int repeat, int flip, int pltt0, u32 addr) {
  *TEXIMAGE_PARAM =
      (u32)((addr >> 3) | (texFmt << 26) | (texGen << 30) | (s << 20) |
            (t << 23) | (repeat << 16) | (flip << 18) | (pltt0 << 29));
}

u32 TexSizeT_ToPixels(u32 sizeCode);
u32 TexSizeS_ToPixels(u32 sizeCode);
void Gx_SendTexturedRect(int x, int y, int z, int w, int h, int s0, int t0,
                         int s1, int t1);

/* Draws a w x h quad showing a whole 16-colour texture whose sides are given
 * as size codes (8 << code pixels). texOffset and paletteOffset count 32-byte
 * blocks of texture and palette memory. flipX and flipY flip the texture. */
void Gx_DrawTexturedQuad(int x, int y, int z, int w, int h, int sizeS,
                         int sizeT, u16 texOffset, u8 paletteOffset, u8 flipX,
                         u8 flipY) {
  u32 texWidth = TexSizeS_ToPixels(sizeS);
  u32 texHeight = TexSizeT_ToPixels(sizeT);

  /* 16 colours, texture coordinates as given, no repeat or flip, colour 0 transparent. */
  G3_TexImageParam(3, 1, sizeS, sizeT, 0, 0, 1, (u32)texOffset << 5);
  *PLTT_BASE = ((u32)paletteOffset << 5) >> 4;
  if (flipX && flipY) {
    Gx_SendTexturedRect(x, y, z, w, h, texWidth, texHeight, 0, 0);
  } else if (flipX) {
    Gx_SendTexturedRect(x, y, z, w, h, texWidth, 0, 0, texHeight);
  } else if (flipY) {
    Gx_SendTexturedRect(x, y, z, w, h, 0, texHeight, texWidth, 0);
  } else {
    Gx_SendTexturedRect(x, y, z, w, h, 0, 0, texWidth, texHeight);
  }
}
