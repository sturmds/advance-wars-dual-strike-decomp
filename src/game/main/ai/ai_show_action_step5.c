#include "types.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

void ActingUnit_FinishAction(void);
void Silo_StartLaunch(int x, int y, int target, int arg, int mode);

void AiShowAction_Step5(void) {
  ActingUnit_FinishAction();
  Silo_StartLaunch(gUnitAction.x, gUnitAction.y, gUnitAction.target,
                   gUnitAction.unk_07, 3);
  gAiTurnState->actionStep = 6;
}
