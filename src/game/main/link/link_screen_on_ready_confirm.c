#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int func_0207bd6c(void);
void LinkScreen_PushReady(void);

/* onConfirm of button 254: queues the ready message unless this console is ready already. */
void LinkScreen_OnReadyConfirm(MenuButton *button)
{
    if (LINK_READY_FLAG(gMenuState, func_0207bd6c()) != 0) {
        return;
    }
    LinkScreen_PushReady();
}
