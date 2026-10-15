#include "types.h"
#include "game/math.h"

/* Darkens slotCount palettes (16 RGB555 colors each) from firstSlot toward
 * black. level 0 leaves them as they are, 16 makes them black. Every
 * channel loses channel * level / 16. The loop shape and declaration order
 * are needed to match. */
void Palette_Darken(u16* palettes, int firstSlot, int slotCount, int level) {
  int i;
  u16* colors;
  int scale;
  u16* p;
  int slot;
  int end;
  scale = HwDiv32(level << 5, 16);
  slot = firstSlot;
  end = firstSlot + slotCount;
  if (slot < end) {
    colors = palettes + slot * 16;
    do {
      for (i = 0, p = colors; i < 16; i++) {
        u16 c = *p;
        int r = c & 0x1f;
        int g = (c >> 5) & 0x1f;
        int b = (c >> 10) & 0x1f;

        r -= r * scale >> 5;
        g -= g * scale >> 5;
        b -= b * scale >> 5;
        *p++ = r | (g << 5) | (b << 10);
      }
      colors += 16;
      slot++;
    } while (slot < end);
  }
}
