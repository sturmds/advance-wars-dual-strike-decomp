/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/keypad.h"
#include "game/menu_button.h"

/* onSelect of the control key buttons: moves the cursor to the key. */
void NameEntry_OnControlKeySelect(MenuButton* button) {
  NameEntry* entry = gNameEntry;

  entry->cursor = button->id - NAME_ENTRY_KEY_BUTTON_ID;
  if ((gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN)) == 0) {
    entry->lastColumn = -1;
  }
}
