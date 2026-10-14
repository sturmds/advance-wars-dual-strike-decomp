#include "types.h"
#include "game/menu_button.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

u8 data_02158f3c[] = {
#include "data/main/data_02158f3c.inc.c"
};

/* Draws the four corners of the menu cursor around a rectangle: sprite
 * tiles 0x3b0, 0x3b2, 0x3b4 and 0x3b6 with palette 15. */
void MenuCursor_DrawAt(int x, int y, int width, int height) {
  int left;
  int top;
  int right;
  int bottom;

  top = y & 0xff;
  left = x & 0x1ff;
  Oam_PutSprite(left, top, data_02158f3c, 0xf3b0);
  right = (x + width) & 0x1ff;
  Oam_PutSprite(right, top, data_02158f3c, 0xf3b2);
  bottom = (y + height) & 0xff;
  Oam_PutSprite(left, bottom, data_02158f3c, 0xf3b4);
  Oam_PutSprite(right, bottom, data_02158f3c, 0xf3b6);
}
