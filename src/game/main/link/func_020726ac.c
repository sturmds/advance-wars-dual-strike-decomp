#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int LinkScreen_FindPanelOfPlayer(int player);
void LinkScreen_MovePlayerToPanel(int player, int panel);

void func_020726ac(int player)
{
    u8 panel;
    u8 lastPanel;

    if (LINK_READY_FLAG(gMenuState, player) != 0) {
        return;
    }
    panel = LinkScreen_FindPanelOfPlayer(player);
    lastPanel = gMenuState.link.playerCount - 1;
    do {
        if (panel != 0) {
            panel = panel - 1;
        } else {
            panel = lastPanel;
        }
    } while (LINK_READY_FLAG(gMenuState, LINK_PANEL_PLAYER(gMenuState, panel)) != 0);
    LinkScreen_MovePlayerToPanel(player, panel);
}
