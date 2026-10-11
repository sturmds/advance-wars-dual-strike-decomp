#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Clears layer 0 and sends it to the screen, on every page but the last. */
void PlayerInfo_ClearTextLayer(void)
{
    if (gMenuState.unk_0206d67c.page == 3) {
        return;
    }
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
}
