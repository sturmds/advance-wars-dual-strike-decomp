/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/menu_button.h"

/* onConfirm of the control key buttons: moves the cursor to the key and
 * raises that key's flag. */
void NameEntry_OnControlKeyConfirm(MenuButton* button) {
  NameEntry* entry = gNameEntry;

  entry->cursor = button->id - NAME_ENTRY_KEY_BUTTON_ID;
  switch (entry->cursor) {
    case NAME_ENTRY_KEY_COUNT + 0:
      entry->unk_124 = 1;
      break;
    case NAME_ENTRY_KEY_COUNT + 1:
      entry->unk_128 = 1;
      break;
    case NAME_ENTRY_KEY_COUNT + 2:
      entry->unk_120 = 1;
      break;
  }
}
