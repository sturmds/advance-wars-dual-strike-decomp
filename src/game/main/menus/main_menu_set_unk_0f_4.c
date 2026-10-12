#include "types.h"
#include "game/main_menu.h"

/* Stores the low four bits of value in MainMenu.unk_0f_4. The (u8) cast and
 * the int parameter are needed to match. */
void MainMenu_SetUnk0f_4(int value) { data_021599d0->unk_0f_4 = (u8)value; }
