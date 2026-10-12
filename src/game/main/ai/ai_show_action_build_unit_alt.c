#include "types.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_action.h"

MapUnit* CurrentPlayer_BuyUnit(int x, int y, int unitType);

/* Display step of action 14: buys the unit type in gUnitAction.unitIndex at
 * the factory tile (x, y), then jumps to the last display step. Same code as action 1; why the AI has a second action is not known. */
void AiShowAction_BuildUnitAlt(void) {
  CurrentPlayer_BuyUnit(gUnitAction.x, gUnitAction.y, gUnitAction.unitIndex);
  gAiTurnState->actionStep = 11;
}
