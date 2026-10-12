#include "types.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0030;
extern s16 gResultPathPoints[];

/* ResultPath_DrawSprite shifted by (dx, dy) pixels. */
void ResultPath_DrawSpriteOffset(int step, int sprite, int dx, int dy) {
  int x;
  int y;

  if (step < 0 || step > 181) {
    return;
  }
  x = dx + (gResultPathPoints[step * 2] - 32);
  if (x >= 256) {
    return;
  }
  y = dy + (gResultPathPoints[step * 2 + 1] - 32);
  if (y <= -64 || y >= 192) {
    return;
  }
  Sprite_QueueAt(7, x & 0x1ff, y & 0xff, &data_027c0030, (sprite + 4) << 12,
                 0x30a + sprite * 64);
}
