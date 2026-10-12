/* Needed to match: the original loads gKeypad->repeat again for left and
 * right, which mwcc only does with common subexpression removal off. */
#pragma opt_common_subs off

#include "types.h"
#include "game/name_entry_work.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void NameEntry_GetCursorPos(NameEntry* entry, s16* x, s16* y);

extern NameEntry* gNameEntry;

#define KEYBOARD_COLUMNS 15
#define KEYBOARD_BOTTOM_ROW 4
#define KEYBOARD_LAST_COLUMN 14

/* onKeys of the keyboard keys (15 columns, 5 rows). Up from the top row
 * and down from the bottom row go to the control key for the column
 * (columns 0-4, 5-9, 10-14); left and right wrap within the row. Saves the
 * new column in lastColumn. */
void NameEntry_MoveFromKey(MenuButton* button) {
  NameEntry* entry = gNameEntry;
  int cursor = entry->cursor;
  s16 x;
  s16 y;

  NameEntry_GetCursorPos(entry, &x, &y);
  if (gKeypad->repeat & PAD_KEY_UP) {
    if (y <= 0) {
      if (x < 5) {
        cursor = NAME_ENTRY_KEY_COUNT;
      } else if (x < 10) {
        cursor = NAME_ENTRY_KEY_COUNT + 1;
      } else {
        cursor = NAME_ENTRY_KEY_COUNT + 2;
      }
      MenuButton_Select(entry->keys[cursor].buttonId);
      entry->cursor = cursor;
      return;
    }
    cursor -= KEYBOARD_COLUMNS;
  } else if (gKeypad->repeat & PAD_KEY_DOWN) {
    if (y >= KEYBOARD_BOTTOM_ROW) {
      if (x < 5) {
        cursor = NAME_ENTRY_KEY_COUNT;
      } else if (x < 10) {
        cursor = NAME_ENTRY_KEY_COUNT + 1;
      } else {
        cursor = NAME_ENTRY_KEY_COUNT + 2;
      }
      MenuButton_Select(entry->keys[cursor].buttonId);
      entry->cursor = cursor;
      return;
    }
    cursor += KEYBOARD_COLUMNS;
  }
  if (gKeypad->repeat & PAD_KEY_LEFT) {
    if (x <= 0) {
      cursor += KEYBOARD_LAST_COLUMN;
    } else {
      cursor--;
    }
  } else if (gKeypad->repeat & PAD_KEY_RIGHT) {
    if (x >= KEYBOARD_LAST_COLUMN) {
      cursor -= KEYBOARD_LAST_COLUMN;
    } else {
      cursor++;
    }
  }
  if (cursor != entry->cursor) {
    MenuButton_Select(entry->keys[cursor].buttonId);
  }
  entry->cursor = cursor;
  NameEntry_GetCursorPos(entry, &x, &y);
  entry->lastColumn = x;
}
