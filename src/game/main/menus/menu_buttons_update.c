#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

MenuButton gMenuButtons[MENU_BUTTON_COUNT];

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Menu_DispatchKeys(void);
BOOL MenuButton_TouchSelect(MenuButton* button);
void MenuCursor_Update(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Runs the menu buttons for one frame, on the screen they were made on:
 * cursor and keys, then touch, then each active button's own update. */
void MenuButtons_Update(void) {
  int i;
  MenuButton* button;
  BOOL done;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx((&data_027c0288.unk_00)[gMenuCursor->screen]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[gMenuCursor->screen]);

  MenuCursor_Update();
  Menu_DispatchKeys();

  button = gMenuButtons;
  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    if (button->active && MenuButton_TouchSelect(button)) {
      break;
    }
  }

  button = gMenuButtons;
  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    if (button->active) {
      MenuButton_UpdateSlide(button);
      MenuButton_Draw(button);
    }
  }

  done = FALSE;
  button = gMenuButtons;
  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    if (button->active) {
      if (button->slideState == MENU_BUTTON_SHOWN) {
        done = MenuButton_HandleTouch(button);
      }
      if (done) {
        break;
      }
    }
  }

  button = gMenuButtons;
  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    if (button->active && button->onUpdate != NULL) {
      button->onUpdate(button);
    }
  }

  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
