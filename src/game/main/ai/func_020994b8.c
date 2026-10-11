#include "types.h"
#include "game/ai_unit_action.h"

int func_020995a4(void);

/* TRUE for mode 1 when the current player has a factory, and for mode 2 when
 * func_020995a4 says so. Nothing calls it. */
BOOL func_020994b8(int mode)
{
    if (mode == 1 && gAiTurnState->factoryDomainsOwn != 0) {
        return TRUE;
    }
    if (mode == 2 && func_020995a4() == 1) {
        return TRUE;
    }
    return FALSE;
}
