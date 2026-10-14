#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Forgets the counter tasks of both sides (both lists, five per side). */
void Battle_ClearCounterTasks(void) {
  int i;
  int j;

  for (i = 0; i < BATTLE_SIDE_COUNT; i++) {
    for (j = 0; j < BATTLE_CELL_COUNT; j++) {
      gMenuState.battle.unk_22c[i][j] = NULL;
      gMenuState.battle.unk_254[i][j] = NULL;
    }
  }
}
