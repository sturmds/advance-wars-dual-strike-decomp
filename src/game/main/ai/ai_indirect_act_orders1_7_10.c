#include "types.h"

void Ai_ClearThreatMaps(void);
void AiUnit_PlanAttack(void);

/* AiUnit_ActByOrderGroup runs this for indirect units with AI order 1, 7
 * or 10. Ai_ClearThreatMaps clears the per-tile AI maps (MapTiles.threatMax and
 * threatSum); AiUnit_PlanAttack picks the attack. */
void AiIndirect_ActOrders1_7_10(void) {
  Ai_ClearThreatMaps();
  AiUnit_PlanAttack();
}
