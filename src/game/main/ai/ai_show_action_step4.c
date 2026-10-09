#include "types.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

void func_020de9c0(int target, int arg);

void AiShowAction_Step4(void)
{
    func_020de9c0(gUnitAction.target, gUnitAction.unk_07);
    gAiTurnState->state = AI_STATE_RUN_QUEUE;
    gAiTurnState->actionStep = 0;
}
