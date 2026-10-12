#include "types.h"
#include "game/menu_button.h"

/* Removes the eight slot buttons (ids 32 to 39). */
void Ov006_RemoveSlotButtons(void) {
  int id;

  for (id = 32; id <= 39; id++) {
    MenuButton_Remove(id);
  }
}
