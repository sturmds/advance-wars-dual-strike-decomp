#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020af5f8(void);
void AiUnit_PlanAttack(void);

/* AiUnit_ActByOrderFog, the queue routine of AiAction_QueueAllDirectUnitsFog,
 * runs this for units with AI order 1 or 10. Same steps as
 * AiFogDirect_ActOtherOrders; AiUnit_PlanAttack picks the attack. */
void AiFogDirect_ActOrders1_10(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
}
