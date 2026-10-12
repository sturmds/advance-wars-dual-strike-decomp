#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_0229fb30.h"

extern MenuButtonTemplate data_02160cc8;
extern MenuButtonTemplate data_02160d40;
extern MenuButtonTemplate data_02160de0;

/* Combat setup buttons: 16-19 one per team row, 32 pixels apart; 254 the
 * Ready button; 252 is added for the link player 0 only (see
 * CombatSetup_SetPlayerUnready). */
void CombatSetup_AddRowButtons(void) {
  int i;

  for (i = 0; i < 4; i++) {
    MenuButton_Add(0, i * 32, &data_02160cc8, i + 16);
  }
  MenuButton_Add(0, 0, &data_02160d40, 254);
  if (data_0229fb30.unk_45a != 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_02160de0, 252);
}
