#include "types.h"
#include "game/menu_state.h"

int LinkScreen_FindPanelOfPlayer(int player);
void LinkScreen_MovePlayerToPanel(int player, int panel);

/* Moves a player down one panel, wrapping to the top and skipping panels of players with the ready flag set. */
void LinkScreen_MovePlayerDown(int player) {
  u8 panel;

  if (LINK_READY_FLAG(gMenuState, player) != 0) {
    return;
  }
  panel = LinkScreen_FindPanelOfPlayer(player);
  do {
    if (panel < gMenuState.link.playerCount - 1) {
      panel = panel + 1;
    } else {
      panel = 0;
    }
  } while (LINK_READY_FLAG(gMenuState, LINK_PANEL_PLAYER(gMenuState, panel)) !=
           0);
  LinkScreen_MovePlayerToPanel(player, panel);
}
