#include "types.h"
#include "game/menu_state.h"

/* Position of a player in the panel order (unk_020 lists the player shown in each panel), or 0 if absent. */
int LinkScreen_FindPanelOfPlayer(int player) {
  int row;

  for (row = 0; row < gMenuState.link.playerCount; row++) {
    if (player == gMenuState.link.panelPlayers[row]) {
      return row;
    }
  }
  return 0;
}
