#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Clears layer 0 and sends it to the screen, on every page but the last. */
void PlayerInfo_ClearTextLayer(void)
{
    if (gMenuState.unk_0206d67c.page == 3) {
        return;
    }
    func_01ff8c84(0);
    Gfx_RequestLayerCopy(0);
}
