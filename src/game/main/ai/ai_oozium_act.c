#include "types.h"

void AiOozium_BuildThreatMaps(void);
void func_020b7460(void);

/* Queue routine for Ooziums, set by AiAction_QueueOoziums. */
void AiOozium_Act(void)
{
    AiOozium_BuildThreatMaps();
    func_020b7460();
}
