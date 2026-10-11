/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/Unk_022a7ea8.h"
#include "game/menu_button.h"

extern NameEntry *data_022a7ea8;

/* onConfirm of the control key buttons: moves the cursor to the key and
 * raises that key's flag. */
void NameEntry_OnControlKeyConfirm(MenuButton *button)
{
    NameEntry *entry = data_022a7ea8;

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
