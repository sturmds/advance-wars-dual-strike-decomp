#include "types.h"
#include "game/keypad.h"
#include "game/main_menu.h"
#include "game/menu_button.h"

int MainMenu_IsFirstSidePage(int page);
void MainMenu_StartPageFlip(int page, int slide);

/* onKeys of the main menu buttons: up and down move the cursor past hidden
 * entries, left, right, L and R flip the page (MainMenu_IsFirstSidePage picks the side),
 * A and B confirm and cancel. */
void MainMenu_HandleKeys(MenuButton* button) {
  MainMenuPage* page;
  u16 keys;
  int cursor;

  cursor = button->id;
  keys = gKeypad->repeat;
  page = *data_0229c71c;
  if (keys & PAD_KEY_UP) {
    do {
      if (cursor == 0) {
        cursor = page->entryCount - 1;
      } else {
        cursor--;
      }
    } while (page->entries[cursor].hidden);
  }
  if (keys & PAD_KEY_DOWN) {
    do {
      if (cursor == page->entryCount - 1) {
        cursor = 0;
      } else {
        cursor++;
      }
    } while (page->entries[cursor].hidden);
  }
  if (keys & (PAD_KEY_LEFT | PAD_BUTTON_L)) {
    data_021599d0->cursors[data_021599d0->page] = cursor;
    if (MainMenu_IsFirstSidePage(data_021599d0->page) == 1) {
      MainMenu_StartPageFlip(6, 2);
    } else {
      MainMenu_StartPageFlip(0, 2);
    }
    return;
  }
  if (keys & (PAD_KEY_RIGHT | PAD_BUTTON_R)) {
    data_021599d0->cursors[data_021599d0->page] = cursor;
    if (MainMenu_IsFirstSidePage(data_021599d0->page) == 1) {
      MainMenu_StartPageFlip(6, 3);
    } else {
      MainMenu_StartPageFlip(0, 3);
    }
    return;
  }
  if (cursor != button->id) {
    button = MenuButton_Select(cursor);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
