#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Clears the first word of each motion and of each entry in the table after it. */
void Battle_ClearMotions(void) {
  int i;
  int j;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 5; j++) {
      gMenuState.battle.motions[i][j].unk_00 = 0;
      gMenuState.battle.unk_13c[i][j].unk_00 = 0;
    }
  }
}
