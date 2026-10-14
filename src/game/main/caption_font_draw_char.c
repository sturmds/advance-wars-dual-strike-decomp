#include "types.h"
#include "game/oam_cells.h"

extern u8 data_0215af8c[];
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int CaptionFont_CharWidth(int ch);

int CaptionFont_DrawChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list) {
  int index;
  u8* entry;

  index = 0;

  if (y & 0x200) {
    x = (((x & 0x1ff) - 16) & 0x1ff) | (x & 0xfe00);
    y = (((y & 0xff) - 16) & 0xff) | (y & 0xff00);
  }
  entry = data_0215af8c;
  if (*entry != 0) {
    do {
      if (*entry == ch) {
        Sprite_Queue(list, x, y, &data_027c0048, attr, firstTile + index * 16);
        return CaptionFont_CharWidth(ch);
      }
      entry++;
      index++;
    } while (*entry != 0);
  }
  return 0;
}
