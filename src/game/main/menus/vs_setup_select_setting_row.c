#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int);
extern MenuState gMenuState;
int VsSetup_ShowSettingHelp(void*);

/* Selects a settings menu row and shows its help text. */
int VsSetup_SelectSettingRow(int row) {
  Sound_PlaySe(0x10002);
  gMenuState.setup.settingRow = row;
  return VsSetup_ShowSettingHelp(&gMenuState);
}
