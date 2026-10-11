#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void CombatSetup_PushMarkReady(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Sends message 9 (mark the local player ready) unless they already are. */
void CombatSetup_PressReady(void)
{
    if (MENU->combat.players[data_0229fb30.unk_45a].ready != 0) {
        return;
    }
    CombatSetup_PushMarkReady();
}
