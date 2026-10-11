#include "types.h"
#include "game/name_entry_work.h"
#include "game/menu_button.h"

extern NameEntry *gNameEntry;

/* onSelect of the letter key buttons: moves the cursor to the key. */
void func_021014e4(MenuButton *button)
{
    gNameEntry->cursor = button->id - NAME_ENTRY_KEY_BUTTON_ID;
}
