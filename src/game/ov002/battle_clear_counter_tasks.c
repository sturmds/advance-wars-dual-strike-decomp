#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Forgets the counter tasks of both sides (both lists, five per side). */
void Battle_ClearCounterTasks(void) {
  int i;
  int j;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 5; j++) {
      gMenuState.battle.unk_22c[i][j] = NULL;
      gMenuState.battle.unk_254[i][j] = NULL;
    }
  }
}
