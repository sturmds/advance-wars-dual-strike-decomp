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

int TexSize_ToPixels(int sizeCode);
int TexSize_ToPixels_02028498(int sizeCode);
void Gx_SendTexturedRect_0202850c(int x, int y, int z, int w, int h, int s0,
                                  int t0, int s1, int t1);

/* Draws a quad the size of a whole texture (sides 8 << size code) in the given
 * texture format, flipped on either axis. texAddr and palAddr are byte
 * offsets into texture and palette memory. */
void Gx_DrawTexturedQuadFormat(int x, int y, int z, int sizeS, int sizeT,
                               int texFmt, u32 texAddr, u32 palAddr, int flipX,
                               int flipY) {
  int width = TexSize_ToPixels_02028498(sizeS);
  int height = TexSize_ToPixels(sizeT);

  /* Texture coordinates as given, no repeat or flip, colour 0 transparent. */
  G3_TexImageParam(texFmt, 1, sizeS, sizeT, 0, 0, 1, texAddr);
  /* 4-colour palettes (format 2) are addressed in 8-byte units, the others in 16. */
  *PLTT_BASE = palAddr >> (4 - (texFmt == 2));
  if (flipX && flipY) {
    Gx_SendTexturedRect_0202850c(x, y, z, width, height, width, height, 0, 0);
  } else if (flipX) {
    Gx_SendTexturedRect_0202850c(x, y, z, width, height, width, 0, 0, height);
  } else if (flipY) {
    Gx_SendTexturedRect_0202850c(x, y, z, width, height, 0, height, width, 0);
  } else {
    Gx_SendTexturedRect_0202850c(x, y, z, width, height, 0, 0, width, height);
  }
}
