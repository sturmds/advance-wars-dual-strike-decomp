#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);
int LinkScreen_FindPanelOfPlayer(int player);

/* Swaps a player with whoever is in the given panel, unless either has its ready flag set. */
void LinkScreen_MovePlayerToPanel(int player, int panel) {
  u8 oldPanel;
  u8 otherPlayer;

  if (LINK_READY_FLAG(gMenuState, player) != 0) {
    return;
  }
  if (LINK_READY_FLAG(gMenuState, LINK_PANEL_PLAYER(gMenuState, panel)) != 0) {
    return;
  }
  oldPanel = LinkScreen_FindPanelOfPlayer(player);
  otherPlayer = LINK_PANEL_PLAYER(gMenuState, panel);
  LINK_PANEL_PLAYER(gMenuState, panel) =
      LINK_PANEL_PLAYER(gMenuState, oldPanel);
  LINK_PANEL_PLAYER(gMenuState, oldPanel) = otherPlayer;
  Sound_PlaySe(0x10002);
}
