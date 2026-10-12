#include "types.h"
#include "game/keypad.h"
#include "game/menu_state.h"

int func_02064acc(void);
int func_02064af0(int row);

extern MenuState gMenuState;

#define SETTING_ROWS 9

/* Keys of the Vs setup help view: left and right pick the setting row
 * (setup.settingRow, wrapping) and show its help text; B closes it. */
void VsSetup_HandleSettingRowKeys(void) {
  s8 row;

  row = gMenuState.setup.settingRow;
  if (gKeypad->repeat & PAD_KEY_LEFT) {
    row--;
    if (row < 0) {
      row = SETTING_ROWS - 1;
    }
    func_02064af0((u8)row);
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    row++;
    if (row >= SETTING_ROWS) {
      row = 0;
    }
    func_02064af0((u8)row);
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_02064acc();
  }
}
