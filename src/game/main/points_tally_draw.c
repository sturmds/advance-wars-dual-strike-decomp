#include "types.h"

extern u8 data_027c0060[];
extern u8 data_027c0108[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void PointsTally_DrawNumber(int x, int y, int value, int palette, int popFrame);

/* Draws the two rows of the points box at (x, y): a label, a number of up
 * to four digits and a suffix each. Only the total in the second row pops in
 * (see PointsTally_DrawDigit); popFrame -1 draws it at rest. */
void PointsTally_Draw(int x, int y, int added, int total, int popFrame) {
  Sprite_QueueAt(6, x & 0x1ff, y & 0xff, data_027c0108, 0x3000, 0xb0);
  PointsTally_DrawNumber(x + 65, y, added, 0, -1);
  Sprite_QueueAt(6, (x + 120) & 0x1ff, y & 0xff, data_027c0060, 0x1000, 0xa8);
  Sprite_QueueAt(6, x & 0x1ff, (y + 16) & 0xff, data_027c0108, 0x2000, 0x60);
  PointsTally_DrawNumber(x + 65, y + 16, total, 1, popFrame);
  Sprite_QueueAt(6, (x + 120) & 0x1ff, (y + 16) & 0xff, data_027c0060, 0x2000,
                 0xa8);
}
