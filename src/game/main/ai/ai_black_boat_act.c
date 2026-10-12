#include "types.h"

void Ai_BuildThreatMaps(void);
void AiBlackBoat_RepairUnit(void);

/* Queue routine for Black Boats, set by AiAction_QueueBlackBoats. */
void AiBlackBoat_Act(void) {
  Ai_BuildThreatMaps();
  AiBlackBoat_RepairUnit();
}
