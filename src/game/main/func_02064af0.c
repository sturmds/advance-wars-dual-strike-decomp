#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int);
extern MenuState gMenuState;
int VsSetup_ShowSettingHelp2(void*);

int func_02064af0(int row) {
  Sound_PlaySe(0x10002);
  gMenuState.setup.settingRow = row;
  return VsSetup_ShowSettingHelp2(&gMenuState);
}
