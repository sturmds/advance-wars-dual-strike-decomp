#include "types.h"
#include "game/ai_unit_action.h"

/* Last entry of both AI unit action tables. */
void AiAction_EndUnitActions(void) {
  gAiTurnState->state = AI_STATE_END_ACTIONS;
}
