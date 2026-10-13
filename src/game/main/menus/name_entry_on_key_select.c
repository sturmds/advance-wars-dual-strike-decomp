/* Needed to match: the original keeps the work pointer in its own register,
 * which mwcc only does with copy propagation off. */
#pragma opt_propagation off

#include "types.h"
#include "game/name_entry_work.h"
#include "game/menu_button.h"

/* onSelect of the letter key buttons: moves the cursor to the key. */
void NameEntry_OnKeySelect(MenuButton* button) {
  NameEntry* entry = gNameEntry;

  entry->cursor = button->id - NAME_ENTRY_KEY_BUTTON_ID;
}
