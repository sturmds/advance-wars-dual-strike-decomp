#include "types.h"

extern u8 data_0215a434[];
extern u8 data_027c0048[];

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int func_02039f60(int ch);

/* Queues the sprite of character `ch`. The tile is firstTile plus 16 per
 * position of `ch` in data_0215a434 (a zero-terminated list). Returns
 * func_02039f60's result for the character, or 0 when it is not in the list. */
int SpriteFont_QueueChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list) {
  int index;
  u8* entry;

  index = 0;
  /* y bit 9 is the OAM double-size flag: the box grows by 16 pixels on
     * each side, so the origin moves back. */
  if (y & 0x200) {
    x = (((x & 0x1ff) - 16) & 0x1ff) | (x & 0xfe00);
    y = (((y & 0xff) - 16) & 0xff) | (y & 0xff00);
  }
  entry = data_0215a434;
  if (*entry != 0) {
    do {
      if (*entry == ch) {
        Sprite_Queue(list, x, y, data_027c0048, attr, firstTile + index * 16);
        return func_02039f60(ch);
      }
      entry++;
      index++;
    } while (*entry != 0);
  }
  return 0;
}
