#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/menu_button.h"

/* Removes the eight slot buttons (ids 32 to 39). */
void Ov006_RemoveSlotButtons(void) {
  int id;

  for (id = OV006_SLOT_BUTTON_FIRST; id <= OV006_SLOT_BUTTON_LAST; id++) {
    MenuButton_Remove(id);
  }
}
