#include "types.h"

/* Blends the 16 colours of palette (RGB555) with color, channel by channel:
 * (palette * palWeight + color * colorWeight) / 16. The weights are meant
 * to add up to 16. The locals are declared outside the loop to match. */
void Palette_BlendWithColor(u16* palette, u16 color, int palWeight,
                            int colorWeight) {
  u16 colorR = color & 0x1f;
  u16 colorG = (color & 0x3e0) >> 5;
  u16 colorB = (color & 0x7c00) >> 10;
  int i;
  u16 c;
  u16 r, g, b;
  int r0 = colorR * colorWeight;
  int g0 = colorG * colorWeight;
  int b0 = colorB * colorWeight;

  for (i = 0; i < 16; i++) {
    c = palette[i];
    r = c & 0x1f;
    g = (c & 0x3e0) >> 5;
    b = (c & 0x7c00) >> 10;
    r = r * palWeight + r0 >> 4;
    g = g * palWeight + g0 >> 4;
    b = b * palWeight + b0 >> 4;
    palette[i] = (r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10);
  }
}
