#include "types.h"
#include "game/BattleSideData.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
/* One 0x50-byte BattleSideData per unit type (the struct itself is shorter, so
 * the table is indexed as bytes). */
extern u8 data_ov002_0236b170[][0x50];

/* Picks the record of each side's unit type. */
void Battle_SelectUnitRecords(int unitType0, int unitType1) {
  gMenuState.battle.unk_27c[0] =
      (BattleSideData*)data_ov002_0236b170[unitType0];
  gMenuState.battle.unk_27c[1] =
      (BattleSideData*)data_ov002_0236b170[unitType1];
}
