/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/keypad.h"
#include "game/menu_button.h"

int NameEntry_FindKey(NameEntry* entry, s16 x, s16 y);

#define KEYBOARD_TOP_ROW 0
#define KEYBOARD_BOTTOM_ROW 4

/* onKeys of the control key buttons. Up and down go to the bottom or top
 * keyboard row, in the column last used there or else the one above this
 * control key; left and right move along the three control keys and wrap. */
void NameEntry_MoveFromControlKey(MenuButton* button) {
  NameEntry* entry = gNameEntry;
  int cursor = entry->cursor;
  s16 column = 0;
  s16 slot;

  switch (entry->cursor - NAME_ENTRY_KEY_COUNT) {
    case 0:
      column = 0;
      break;
    case 1:
      column = 5;
      break;
    case 2:
      column = 10;
      break;
  }
  if (gKeypad->repeat & PAD_KEY_UP) {
    if (entry->lastColumn != -1) {
      column = entry->lastColumn;
    }
    cursor = NameEntry_FindKey(entry, column, KEYBOARD_BOTTOM_ROW);
    MenuButton_Select(entry->keys[cursor].buttonId);
    entry->cursor = cursor;
  } else if (gKeypad->repeat & PAD_KEY_DOWN) {
    if (entry->lastColumn != -1) {
      column = entry->lastColumn;
    }
    cursor = NameEntry_FindKey(entry, column, KEYBOARD_TOP_ROW);
    MenuButton_Select(entry->keys[cursor].buttonId);
    entry->cursor = cursor;
  } else {
    slot = entry->cursor - NAME_ENTRY_KEY_COUNT;
    if (gKeypad->repeat & PAD_KEY_LEFT) {
      if (slot <= 0) {
        cursor += 2;
      } else {
        cursor--;
      }
    } else if (gKeypad->repeat & PAD_KEY_RIGHT) {
      if (slot >= 2) {
        cursor -= 2;
      } else {
        cursor++;
      }
    }
    if (cursor != entry->cursor) {
      MenuButton_Select(entry->keys[cursor].buttonId);
    }
    entry->cursor = cursor;
  }
}
