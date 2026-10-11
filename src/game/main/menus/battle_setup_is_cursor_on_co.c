#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* 1 when the cursor (setup.cursorButton) is on a CO, ids 8-15. A switch, not a
 * range test, to match. */
int BattleSetup_IsCursorOnCo(void)
{
    switch (gMenuState.setup.cursorButton) {
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return 1;
    default:
        return 0;
    }
}
