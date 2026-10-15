#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_02158f54;
extern MenuButtonTemplate data_ov000_0234f28c;
extern MenuButtonTemplate data_ov000_0234f2b4;

void TpIcon2_LoadTo(u8* dest);

/* Loads the button graphics into object VRAM, then adds a reserved button and
 * two buttons at x 112, y 32 and y 128 (ids 0 and 1) and selects the first. */
void UnitInfoPanel_AddButtons(void) {
  TpIcon2_LoadTo(data_027c0280->objVram + 0x3280);
  MenuButton_AddReserved(0, 0, &data_02158f54);
  MenuButton_Add(112, 32, &data_ov000_0234f28c, 0);
  MenuButton_Add(112, 128, &data_ov000_0234f2b4, 1);
  MenuButton_Select(0);
}
