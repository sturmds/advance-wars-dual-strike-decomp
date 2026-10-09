#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020abe3c(void);

/* Queue routine for APCs and T-Copters that carry a unit, set by
 * AiAction_QueueTransportUnloads. */
void AiTransport_Unload(void)
{
    Ai_BuildThreatMaps();
    func_020abe3c();
}
