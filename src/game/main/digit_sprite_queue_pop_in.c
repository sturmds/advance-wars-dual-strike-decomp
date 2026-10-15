#include "types.h"
#include "game/oam_cells.h"
#include "game/oam_buffer.h"
#include "game/math.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Queues one digit sprite at (x, y). Frames 0 to 8 play a pop-in: the sprite
 * is drawn through affine matrix `matrixIndex`, scaled by an ease from 2x
 * down to 1x, and moves from sprite list 1 to list 5 as it shrinks. Any
 * other frame draws it plain on list 6. gSineTable has 256 steps per
 * turn (index 64 = cosine of 0). */
void DigitSprite_QueuePopIn(int x, int y, int digit, int matrixIndex,
                            int frame) {
  int scale;
  int list;

  list = 5;
  if (frame < 0 || frame > 8) {
    Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c0028, 0xc000,
                   0x330 + digit * 2);
    return;
  }
  scale = Math_Ease(4, 0x200, 0x100, frame, 8);
  /* Needed to match: the divisor is evaluated once per entry, before the
     * table reads. */
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int cosValue = gSineTable[64] * 16;
    int sinValue = gSineTable[0];

    Oam_SetAffineParams(matrixIndex, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                        HwDiv32(cosValue, d3));
  }
  list -= (scale - 0x100) >> 6;
  Sprite_Queue(list, ((x - 4) & 0x1ff) | (matrixIndex << 9),
               ((y - 4) & 0xff) | 0x300, &data_027c0028, 0xc000,
               0x330 + digit * 2);
}
