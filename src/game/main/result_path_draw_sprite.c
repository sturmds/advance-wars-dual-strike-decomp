#include "types.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0030;
extern s16 gResultPathPoints[];

/* Queues result-screen sprite `sprite` (0 to 3) at step `step` of the path
 * in gResultPathPoints (0 to 181; x, y pairs, the sprite is 64x64 and centred
 * on the point). Steps whose box lies off screen draw nothing. */
void ResultPath_DrawSprite(int step, int sprite) {
  int x;
  int y;

  if (step < 0 || step > 181) {
    return;
  }
  x = gResultPathPoints[step * 2] - 32;
  if (x >= 256) {
    return;
  }
  y = gResultPathPoints[step * 2 + 1] - 32;
  if (y <= -64 || y >= 192) {
    return;
  }
  Sprite_QueueAt(7, x & 0x1ff, y & 0xff, &data_027c0030, (sprite + 4) << 12,
                 0x30a + sprite * 64);
}
