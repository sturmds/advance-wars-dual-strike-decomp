#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* First tile row of a player panel; panels are 4 rows tall and centred for 2 to 4 players. */
int LinkScreen_GetPlayerRow(int player) {
  switch (gMenuState.link.playerCount) {
    case 4:
      return player * 4 + 4;
    case 3:
      return player * 4 + 6;
    default:
      return player * 4 + 8;
  }
}
