#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
int Battle_CompareHpToCellStep(int side, int shownHp, int index);

/* Sets the five HP cells of a side from its shown HP (1 to 10) before and after the fight. */
void Battle_InitHpCells(int side, int shownHpBefore, int shownHpAfter) {
  int i;

  for (i = 0; i < BATTLE_CELL_COUNT; i++) {
    gMenuState.battle.unk_310[side][i].fillBefore =
        Battle_CompareHpToCellStep(side, shownHpBefore, i);
    gMenuState.battle.unk_310[side][i].fillShown =
        gMenuState.battle.unk_310[side][i].fillBefore;
    gMenuState.battle.unk_310[side][i].fillAfter =
        Battle_CompareHpToCellStep(side, shownHpAfter, i);
  }
}
