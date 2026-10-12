#include "types.h"

void AiOozium_BuildThreatMaps(void);
void AiOozium_MoveToTarget(void);

/* Queue routine for Ooziums, set by AiAction_QueueOoziums. */
void AiOozium_Act(void) {
  AiOozium_BuildThreatMaps();
  AiOozium_MoveToTarget();
}
