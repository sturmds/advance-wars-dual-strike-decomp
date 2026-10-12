/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_button.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const MenuButtonTemplate data_0216ab04;
extern const MenuButtonTemplate data_0216ab2c;

/* Adds a menu button for every used key, character keys first, then the
 * control keys, and selects the button under the cursor. */
void NameEntry_AddKeyButtons(NameEntry* entry) {
  int i;
  int j;
  NameEntryKey* key;

  for (i = 0; i < NAME_ENTRY_KEY_COUNT; i++) {
    key = &entry->keys[i];
    if (key->unk_00 != 0) {
      key->buttonId = i + NAME_ENTRY_KEY_BUTTON_ID;
      {
        Unk_027c0280* saved0280 = data_027c0280;
        Unk_02080440* saved027c;

        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        MenuButton_Add(key->buttonX - 2, key->buttonY, &data_0216ab04,
                       key->buttonId);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
      }
    }
  }
  /* The control keys follow keys[] in the work block; needed to match: i
     * still holds NAME_ENTRY_KEY_COUNT from the loop above. */
  for (j = 0; j < NAME_ENTRY_CONTROL_KEY_COUNT; j++) {
    key = &entry->keys[i + j];
    if (key->unk_00 != 0) {
      key->buttonId = i + NAME_ENTRY_KEY_BUTTON_ID + j;
      {
        Unk_027c0280* saved0280 = data_027c0280;
        Unk_02080440* saved027c;

        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        MenuButton_Add(key->buttonX, key->buttonY, &data_0216ab2c,
                       key->buttonId);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
      }
    }
  }
  MenuButton_Select(entry->keys[entry->cursor].buttonId);
}
