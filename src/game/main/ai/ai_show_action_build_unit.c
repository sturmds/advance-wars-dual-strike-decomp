#include "types.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

int func_020c716c(int x, int y, int unitType);

/* Display step of action 1: buys the unit type in gUnitAction.unitIndex at
 * the factory tile (x, y), then jumps to the last display step. */
void AiShowAction_BuildUnit(void)
{
    func_020c716c(gUnitAction.x, gUnitAction.y, gUnitAction.unitIndex);
    gAiTurnState->actionStep = 11;
}
