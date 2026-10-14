#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

extern const MenuButtonTemplate data_ov006_0236b684;
extern Unk_ov006_0235f690* data_ov006_0236d640;

/* On screen 0, adds the eight slot buttons (ids 32 to 39, tiles 16 apart) and
 * resets their slots: off screen at x = -32, no speed, state 0. */
void Ov006_AddSlotButtons(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  int i = 0;
  int id;
  MenuButton* button;
  Ov006SlotButton* slot;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (id = OV006_SLOT_BUTTON_FIRST; id <= OV006_SLOT_BUTTON_LAST; id++) {
    button = MenuButton_Add(0, 0, &data_ov006_0236b684, id);
    button->tile = 0x25e + i * 16;
    slot = &data_ov006_0236d640->slotButtons[id - OV006_SLOT_BUTTON_FIRST];
    slot->unk_0a = 32;
    slot->state = 0;
    slot->flags = 0;
    slot->x = -0x20000;
    slot->speed = 0;
    i++;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
