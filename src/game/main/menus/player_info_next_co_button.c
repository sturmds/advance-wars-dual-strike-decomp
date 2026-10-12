#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Id of the next CO button after id (see PlayerInfo_AddCoButtons), skipping
 * COs without one and wrapping from the last to the first. */
int PlayerInfo_NextCoButton(int id) {
  int last = gMenuState.unk_0206d67c.playerCount * 2 - 1;

  do {
    if (id == last) {
      id = 0;
    } else {
      id++;
    }
  } while (gMenuState.unk_0206d67c.coShown[id >> 1][id & 1] == 0);
  return id;
}
