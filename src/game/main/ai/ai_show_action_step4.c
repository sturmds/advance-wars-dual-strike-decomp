#include "types.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

void ActingUnit_MoveAndAttackObject(int target, int arg);

void AiShowAction_Step4(void) {
  ActingUnit_MoveAndAttackObject(gUnitAction.target, gUnitAction.unk_07);
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}
