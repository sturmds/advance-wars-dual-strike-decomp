#include "types.h"
#include "game/BattleSideData.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
/* One BattleSideData per unit type. */
extern BattleSideData data_ov002_0236b170[];

/* Picks the record of each side's unit type. */
void Battle_SelectUnitRecords(int unitType0, int unitType1) {
  gMenuState.battle.unk_27c[0] = &data_ov002_0236b170[unitType0];
  gMenuState.battle.unk_27c[1] = &data_ov002_0236b170[unitType1];
}
