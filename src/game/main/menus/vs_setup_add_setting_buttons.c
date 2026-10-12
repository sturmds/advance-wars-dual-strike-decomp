#include "types.h"
#include "game/menu_button.h"

#define SETTING_ROWS 9

extern u8 gVsSetupSettingRowY[SETTING_ROWS];
extern u8 gVsSetupSettingRowX[SETTING_ROWS];
extern MenuButtonTemplate data_0215cc88;
extern MenuButtonTemplate data_0215cd78;

/* Adds button 254 and, per setting row of the Vs setup screen, the row
 * button (id 60 + row). */
void VsSetup_AddSettingButtons(void) {
  int row;

  MenuButton_Add(0, 0, &data_0215cc88, 254);
  for (row = 0; row < SETTING_ROWS; row++) {
    MenuButton_Add(gVsSetupSettingRowX[row], gVsSetupSettingRowY[row],
                   &data_0215cd78, row + 60);
  }
}
