#include "types.h"
#include "game/menu_button.h"

int Link_IsActive(void);
int Link_GetOwnPlayer(void);

#define SETTING_ROWS 9

extern u8 gVsSetupSettingRowY[SETTING_ROWS];
extern u8 gVsSetupSettingRowX[SETTING_ROWS];
extern MenuButtonTemplate data_0215caf8;
extern MenuButtonTemplate data_0215cbc0;
extern MenuButtonTemplate data_0215cd00;
extern MenuButtonTemplate data_0215cd28;
extern MenuButtonTemplate data_0215cdc8;

/* Adds buttons 254 and 253 and, per setting row of the Vs setup screen, the
 * row button (id 60 + row) and two buttons 16 pixels above and 32 below it
 * (ids 69 + row * 2 and 70 + row * 2; arrows is a guess). In a link game
 * only the first link player gets them. */
void VsSetup_AddSettingButtonsAndArrows(void) {
  int row;

  if (Link_IsActive() == 1 && Link_GetOwnPlayer() != 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_0215cd00, 254);
  MenuButton_Add(0, 0, &data_0215cdc8, 253);
  for (row = 0; row < SETTING_ROWS; row++) {
    MenuButton_Add(gVsSetupSettingRowX[row], gVsSetupSettingRowY[row],
                   &data_0215cd28, row + 60);
    MenuButton_Add(gVsSetupSettingRowX[row] + 8, gVsSetupSettingRowY[row] - 16,
                   &data_0215caf8, row * 2 + 69);
    MenuButton_Add(gVsSetupSettingRowX[row] + 8, gVsSetupSettingRowY[row] + 32,
                   &data_0215cbc0, row * 2 + 70);
  }
}
