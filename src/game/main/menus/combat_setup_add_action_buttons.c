#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"

extern MenuButtonTemplate data_02161420;
extern MenuButtonTemplate data_02161448;

/* Adds button 14 when the layout has cells in use (unk_18 is not 0), and
 * button 15 always. A guess: these are the action buttons of a Combat
 * setup screen. */
void CombatSetup_AddActionButtons(void) {
  if (data_02160ac0->cellCount != 0) {
    MenuButton_Add(0, 0, &data_02161420, 14);
  }
  MenuButton_Add(0, 0, &data_02161448, 15);
}
