#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

/* Fills button from template, placed at (x, y), and makes it active. Also
 * records which screen is current, for MenuButtons_Update. */
MenuButton* MenuButton_Init(int x, int y, MenuButton* button,
                            const MenuButtonTemplate* template, int id) {
  button->x = x + template->x;
  button->y = y + template->y;
  button->width = template->width;
  button->height = template->height;
  button->flags = template->flags;
  button->slideDuration = template->slideDuration;
  button->tile = template->tile;
  button->palette = template->palette;
  button->onUpdate = template->onUpdate;
  button->onKeys = template->onKeys;
  button->onConfirm = template->onConfirm;
  button->onCancel = template->onCancel;
  button->onSelect = template->onSelect;
  button->onSlideOutDone = template->onSlideOutDone;
  button->id = id;
  button->touchState = 0;
  button->slideState = 0;
  button->slideFromX = button->slideToX = button->x;
  button->slideFromY = button->slideToY = button->y;
  button->slideTimer = 0;
  button->active = 1;

  if (data_027c027c == data_027c0288.unk_00) {
    gMenuCursor->screen = 0;
  } else {
    gMenuCursor->screen = 1;
  }
  return button;
}
