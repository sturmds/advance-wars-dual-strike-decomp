#include "types.h"
#include "game/math.h"

extern u8 data_02168660[];
extern u8 data_02168678[];
extern u8 data_0216870c[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Main-module copy of PowerMeter_QueueStars (ov000) for the power panel on
 * the right half of the screen: stars go right to left, always in object
 * palette 7. charge is consumed; each star spans starSize charge points
 * and has four fill frames, frame 3 for a full star. style 0 is the CO
 * Power stretch, style 1 the Super CO Power stretch. With bob set, the
 * stars bounce along a table indexed by *bobCounter, which counts down. */
void PowerPanelRight_QueueStars(int* x, int* y, int* charge, int starSize,
                                int length, int style, int bob,
                                int* bobCounter) {
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
      sprite = data_02168660;
      stride = 1;
      base = 24;
    } else {
      sprite = data_02168678;
      stride = 2;
      base = 28;
    }
    if (bob != 0) {
      yOffset = data_0216870c[HwMod32((*bobCounter)--, 32)];
    } else {
      yOffset = 0;
    }
    Sprite_QueueAt(0, *x, *y - yOffset, sprite,
                   (7 << 12) | (frame * stride + base), 0);
    *x -= 6;
    length -= starSize;
  }
}
