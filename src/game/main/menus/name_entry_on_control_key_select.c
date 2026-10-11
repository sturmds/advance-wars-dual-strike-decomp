/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/Unk_022a7ea8.h"
#include "game/keypad.h"
#include "game/menu_button.h"

extern NameEntry *data_022a7ea8;

/* onSelect of the control key buttons: moves the cursor to the key. */
void NameEntry_OnControlKeySelect(MenuButton *button)
{
    NameEntry *entry = data_022a7ea8;

    entry->cursor = button->id - NAME_ENTRY_KEY_BUTTON_ID;
    if ((gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN)) == 0) {
        entry->unk_108 = -1;
    }
}
