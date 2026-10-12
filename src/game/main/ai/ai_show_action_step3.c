#include "types.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

void func_020dea20(int target);

void AiShowAction_Step3(void) {
  func_020dea20(gUnitAction.target);
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}
