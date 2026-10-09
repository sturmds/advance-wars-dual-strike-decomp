#include "types.h"
#include "game/ai_unit_action.h"

void func_0209c948(void);
void func_020a9854(void);
void func_020ab014(void);
void func_020aba28(void);
void func_020abe3c(void);
void func_020b2904(void);

/* Queue routine for APCs and T-Copters that carry a unit, set by
 * AiAction_QueueLoadedTransports. func_0209fc10 runs
 * func_020abe3c alone on the same units. */
void AiLoadedTransport_Act(void)
{
    func_0209c948();
    func_020aba28();
    func_020ab014();
    gAiTurnState->unk_08 |= 0x20;
    func_020abe3c();
    func_020aba28();
    func_020a9854();
    func_020b2904();
}
