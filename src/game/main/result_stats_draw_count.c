#include "types.h"
#include "game/oam_cells.h"
#include "game/math.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

#define MARK_TILE 32
#define DIGIT_TILE(digit) ((digit) * 10 + 34)
/* Digits 1-3 are drawn one pixel further right than the others. */
#define IS_NARROW_DIGIT(digit) ((u32)((digit) - 1) <= 2)

/* Draws a count of up to 99 at (x, y): a mark sprite, then one or two
 * digits. Larger values show as 99. */
void ResultStats_DrawCount(int x, int y, int value) {
  int tens;

  if (value > 99) {
    value = 99;
  }
  Sprite_QueueAt(0, (x + 2) & 0x1ff, (y + 7) & 0xff, &data_027c0028, 0x1000,
                 MARK_TILE);
  if (value >= 10) {
    tens = HwDiv32(value, 10);
    value = HwMod32(value, 10);
    if (IS_NARROW_DIGIT(tens)) {
      Sprite_QueueAt(0, (x + 12) & 0x1ff, (y + 1) & 0xff, &data_027c0038,
                     0x1000, DIGIT_TILE(tens));
    } else {
      Sprite_QueueAt(0, (x + 11) & 0x1ff, (y + 1) & 0xff, &data_027c0038,
                     0x1000, DIGIT_TILE(tens));
    }
  }
  if (IS_NARROW_DIGIT(value)) {
    Sprite_QueueAt(0, (x + 20) & 0x1ff, (y + 1) & 0xff, &data_027c0038, 0x1000,
                   DIGIT_TILE(value));
  } else {
    Sprite_QueueAt(0, (x + 19) & 0x1ff, (y + 1) & 0xff, &data_027c0038, 0x1000,
                   DIGIT_TILE(value));
  }
}
