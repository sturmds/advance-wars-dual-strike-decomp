#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020af18c(void);

/* Queue routine for Black Boats, set by AiAction_QueueBlackBoats. */
void AiBlackBoat_Act(void)
{
    Ai_BuildThreatMaps();
    func_020af18c();
}
