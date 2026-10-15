#include "types.h"
#include "game/math.h"

extern u8 data_ov000_0234ebe8[];
extern u8 data_ov000_0234ebf8[];
extern u8 data_ov000_0234ec4c[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues the stars of one stretch of the power meter, right to left.
 * charge is the meter value still to draw and is consumed. Each star spans
 * starSize charge points and has four fill frames, frame 3 for a full
 * star. length is the stretch's cost in charge points; style 0 is the CO
 * power stretch, style 1 the super power stretch. With bob set, the stars
 * bounce along a table indexed by *bobCounter, which counts down. */
void PowerMeter_QueueStars(int* x, int* y, int* charge, int starSize,
                           int length, int style, int bob, int* bobCounter,
                           int palette) {
  int frame;
  void* sprite;
  int stride;
  int base;
  int yOffset;
  int quarterStar = HwDiv32(starSize, 4);

  while (length > 0) {
    if (*charge >= starSize) {
      *charge -= starSize;
      frame = 3;
    } else {
      frame = HwDiv32(*charge, quarterStar);
      if (frame >= 3) {
        frame = 2;
      }
      *charge = 0;
    }
    if (style == 0 || style != 1) {
      sprite = data_ov000_0234ebe8;
      stride = 1;
      base = 8;
    } else {
      sprite = data_ov000_0234ebf8;
      stride = 2;
      base = 12;
    }
    if (bob != 0) {
      yOffset = data_ov000_0234ec4c[HwMod32((*bobCounter)--, 32)];
    } else {
      yOffset = 0;
    }
    Sprite_QueueAt(0, *x, *y - yOffset, sprite,
                   (palette << 12) | (frame * stride + base), 0);
    *x -= 6;
    length -= starSize;
  }
}
