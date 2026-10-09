#include "types.h"
#include "game/ai_unit_action.h"

void Ai_BuildThreatMaps(void);
void func_020a9854(void);
void AiUnit_SeekSeaTransport(void);
void func_020aba28(void);
void func_020abe3c(void);
void AiUnit_WaitInPlace(void);

/* Queue routine for APCs and T-Copters that carry a unit, set by
 * AiAction_QueueLoadedTransports. AiTransport_Unload runs
 * func_020abe3c alone on the same units. */
void AiLoadedTransport_Act(void)
{
    Ai_BuildThreatMaps();
    func_020aba28();
    AiUnit_SeekSeaTransport();
    gAiTurnState->stepFlags |= AI_STEP_ATTACK_DONE;
    func_020abe3c();
    func_020aba28();
    func_020a9854();
    AiUnit_WaitInPlace();
}
