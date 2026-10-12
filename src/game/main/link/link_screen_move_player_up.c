/* Built at optimization level 2 (the pragma sweep found it); level 4 moves the
 * gMenuState load above the panel narrowing. */
#pragma optimization_level 2
#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int LinkScreen_FindPanelOfPlayer(int player);
void LinkScreen_MovePlayerToPanel(int player, int panel);

/* Moves a player up one panel, wrapping to the bottom and skipping panels of players with the ready flag set. */
void LinkScreen_MovePlayerUp(int player) {
  u8 panel;

  if (LINK_READY_FLAG(gMenuState, player) != 0) {
    return;
  }
  panel = LinkScreen_FindPanelOfPlayer(player);
  do {
    if (panel != 0) {
      panel = panel - 1;
    } else {
      panel = gMenuState.link.playerCount - 1;
    }
  } while (LINK_READY_FLAG(gMenuState, LINK_PANEL_PLAYER(gMenuState, panel)) !=
           0);
  LinkScreen_MovePlayerToPanel(player, panel);
}
