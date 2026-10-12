#include "types.h"
#include "game/break_text.h"

extern u8 data_027c0000[];
extern u8 data_027c0048[];
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

/* Like BreakText_QueueLetter, for letter cells of either size: 32 x 32
 * pixels for the tag power font, 16 x 16 otherwise. With bit 9 of y set the
 * sprite moves left by half a cell and 16 up. */
void BreakText_QueueLetterSized(u8 index, u16 x, u16 y, int attr, u8 list) {
  int cellTiles;

  if (y & 0x200) {
    cellTiles = gBreakText->tagFont == 1 ? 4 : 2;
    x = ((x & 0x1ff) - ((cellTiles << 3) >> 1)) & 0x1ff | (x & 0xfe00);
    y = ((y & 0xff) - 16) & 0xff | (y & 0xff00);
  }
  cellTiles = gBreakText->tagFont == 1 ? 4 : 2;
  Sprite_Queue(list, x, y,
               gBreakText->tagFont == 1 ? data_027c0048 : data_027c0000,
               attr | gBreakText->palette << 12,
               index * (cellTiles << 2) + gBreakText->tile);
}
