#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int func_0207bd6c(void);
void LinkScreen_PushMoveToPanel(int panel);

/* onConfirm of the panel buttons (id = panel): asks to move this console's player to that panel, unless it is ready or already there. */
void LinkScreen_OnPlayerButtonConfirm(MenuButton *button)
{
    u8 panel;

    if (LINK_READY_FLAG(gMenuState, func_0207bd6c()) != 0) {
        return;
    }
    panel = button->id;
    if (LINK_PANEL_PLAYER(gMenuState, panel) == func_0207bd6c()) {
        return;
    }
    LinkScreen_PushMoveToPanel(panel);
}
