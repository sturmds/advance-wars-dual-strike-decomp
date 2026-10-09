#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020a9854(void);
void func_020ac648(void);
void func_020ac9e8(void);
void func_020af5f8(void);
void AiUnit_WaitInPlace(void);

/* Queue routine for APCs and T-Copters that carry nothing, set by
 * AiAction_QueueEmptyTransports. */
void AiEmptyTransport_Act(void)
{
    Ai_BuildThreatMaps();
    func_020af5f8();
    func_020ac9e8();
    func_020ac648();
    func_020a9854();
    AiUnit_WaitInPlace();
}
