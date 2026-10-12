#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void func_0208acb0(void);
void CombatModeMenu_DrawFundsAndPrices(void);

#define BUTTON_FIRST_UNIT 8
#define MAX_UNITS 20
#define SE_CONFIRM 0x10006

/* onConfirm of the four Combat mode shop entries (buttons 8-11): unless
 * MAX_UNITS cells are in use, adds the entry's unit (cell value 1, 3, 4 or
 * 5) for 1000, 3000, 5000 or 6000 of the Combat mode funds. The first unit
 * bought also adds button 14 (func_0208acb0). */
void CombatModeMenu_BuyUnit(MenuButton* button) {
  if (data_02160ac0->cellCount == MAX_UNITS) {
    return;
  }
  switch (button->id - BUTTON_FIRST_UNIT) {
    case 0:
      if (data_02160ac0->funds < 1000) {
        return;
      }
      data_02160ac0->cells[data_02160ac0->cellCount].tile = 1;
      data_02160ac0->funds -= 1000;
      break;
    case 1:
      if (data_02160ac0->funds < 3000) {
        return;
      }
      data_02160ac0->cells[data_02160ac0->cellCount].tile = 3;
      data_02160ac0->funds -= 3000;
      break;
    case 2:
      if (data_02160ac0->funds < 5000) {
        return;
      }
      data_02160ac0->cells[data_02160ac0->cellCount].tile = 4;
      data_02160ac0->funds -= 5000;
      break;
    case 3:
      if (data_02160ac0->funds < 6000) {
        return;
      }
      data_02160ac0->cells[data_02160ac0->cellCount].tile = 5;
      data_02160ac0->funds -= 6000;
      break;
  }
  if (data_02160ac0->cellCount == 0) {
    func_0208acb0();
  }
  data_02160ac0->cellCount++;
  CombatModeMenu_DrawFundsAndPrices();
  Sound_PlaySe(SE_CONFIRM);
}
