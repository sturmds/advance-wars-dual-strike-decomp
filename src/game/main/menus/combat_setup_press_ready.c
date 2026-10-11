#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void func_02085d60(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Sends message 9 (mark the local player ready) unless they already are. */
void CombatSetup_PressReady(void)
{
    if (MENU->combat.players[data_0229fb30.unk_45a].unk_1 != 0) {
        return;
    }
    func_02085d60();
}
