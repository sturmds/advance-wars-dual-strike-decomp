#include "types.h"
#include "game/event_queue.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Clears the ready flags of the four slots (setup.ready) and the event queue. */
void VsSetup_ClearReadyFlags(void)
{
    int slot;

    for (slot = 0; slot < 4; slot++) {
        gMenuState.setup.ready[slot] = 0;
    }
    EventQueue_Reset();
}
