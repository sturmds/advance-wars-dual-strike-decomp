#include "types.h"
#include "game/menu_button.h"

int MenuButton_TestFlags(MenuButton* entry, int mask);
const void* Oam_GetShapeCells(int width, int height);
void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

/* Puts the button's sprite into OAM. Bit 10 of OAM attribute 0 selects
 * semi-transparent mode. */
void MenuButton_Draw(MenuButton* button) {
  u16 tile;
  u16 palette;
  s16 width;
  s16 height;
  int mode;

  tile = button->tile;
  palette = button->palette;
  width = button->width;
  height = button->height;
  mode = 0;
  if (MenuButton_TestFlags(button, MENU_BUTTON_VISIBLE) == 0) {
    return;
  }
  if (MenuButton_TestFlags(button, MENU_BUTTON_SEMI_TRANSPARENT) != 0) {
    mode = 0x400;
  }
  Oam_PutCells(button->x & 0x1ff, (button->y & 0xff) | mode,
               Oam_GetShapeCells(width, height), palette << 12, tile);
}
