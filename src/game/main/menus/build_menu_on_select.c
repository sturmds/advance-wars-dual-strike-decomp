#include "types.h"
#include "game/build_menu.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"

void UnitInfoPanel_OpenForTypeOnSecondScreen(int owner, int unitType);
void Sound_PlaySe(int id);

/* Select callback of a build menu row: shows the unit's info on the other
 * screen and plays a sound. */
void BuildMenu_OnSelect(MenuButton* button) {
  UnitInfoPanel_OpenForTypeOnSecondScreen(
      MAP_STATE->currentPlayer, gBuildMenuEntries[button->id].unitType);
  Sound_PlaySe(0x10002);
}
