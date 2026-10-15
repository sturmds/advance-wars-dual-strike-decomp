#include "types.h"
#include "game/oam_cells.h"
#include "game/oam_buffer.h"
#include "game/math.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Queues one large digit sprite at (x, y) in palette palette + 1, like
 * DigitSprite_QueuePopIn: frames 0 to 8 shrink it from 2x to 1x through
 * affine matrix `matrix` and move it from sprite list 2 to list 6; any
 * other frame draws it plain on list 6. */
void PointsTally_DrawDigit(int x, int y, int digit, int palette, int matrix,
                           int popFrame) {
  int scale;
  int list;

  list = 6;
  if (popFrame < 0 || popFrame > 8) {
    Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c0040, (palette + 1) << 12,
                   digit * 4 + 0x80);
    return;
  }
  scale = Math_Ease(4, 0x200, 0x100, popFrame, 8);
  /* Needed to match: the divisor is evaluated once per entry, before the
     * table reads. */
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int cosValue = gSineTable[64] * 16;
    int sinValue = gSineTable[0];

    Oam_SetAffineParams(matrix, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                        HwDiv32(cosValue, d3));
  }
  list -= (scale - 0x100) >> 6;
  Sprite_Queue(list, ((x - 8) & 0x1ff) | (matrix << 9),
               ((y - 8) & 0xff) | 0x300, &data_027c0040, (palette + 1) << 12,
               digit * 4 + 0x80);
}
