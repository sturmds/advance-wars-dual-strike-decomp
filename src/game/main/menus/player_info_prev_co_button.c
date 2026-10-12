#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Id of the previous CO button before id (see PlayerInfo_AddCoButtons),
 * skipping COs without one and wrapping from the first to the last. */
int PlayerInfo_PrevCoButton(int id) {
  int last = gMenuState.unk_0206d67c.playerCount * 2 - 1;

  do {
    if (id == 0) {
      id = last;
    } else {
      id--;
    }
  } while (gMenuState.unk_0206d67c.coShown[id >> 1][id & 1] == 0);
  return id;
}
