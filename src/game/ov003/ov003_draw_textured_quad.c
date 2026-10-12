#include "types.h"

#define TEXIMAGE_PARAM ((volatile u32*)0x040004a8)
#define PLTT_BASE ((volatile u32*)0x040004ac)

u32 func_ov003_023538c4(u32 index);
u32 func_ov003_02353908(u32 index);
void func_ov003_0235394c(int x, int y, int z, int w, int h, int s0, int t0,
                         int s1, int t1);

/* Draws a width x height quad (sides 8 << size, from the two size codes) with a
 * 3D engine texture and palette. The texture parameter sets 16-colour format,
 * texture coordinate transform mode and transparent colour 0; the size codes
 * are the texture's S and T size fields. flipX and flipY flip the texture. */
void Ov003_DrawTexturedQuad(int x, int y, int z, int widthSize, int heightSize,
                            int texOffset, int paletteOffset, int flipX,
                            int flipY) {
  u32 width = func_ov003_02353908(widthSize);
  u32 height = func_ov003_023538c4(heightSize);

  *TEXIMAGE_PARAM = (((u32)texOffset << 5) >> 3) | 0x4c000000 |
                    (widthSize << 20) | (heightSize << 23) | 0x20000000;
  *PLTT_BASE = ((u32)paletteOffset << 5) >> 4;
  if (flipX && flipY) {
    func_ov003_0235394c(x, y, z, width, height, width, height, 0, 0);
  } else if (flipX) {
    func_ov003_0235394c(x, y, z, width, height, width, 0, 0, height);
  } else if (flipY) {
    func_ov003_0235394c(x, y, z, width, height, 0, height, width, 0);
  } else {
    func_ov003_0235394c(x, y, z, width, height, 0, 0, width, height);
  }
}
