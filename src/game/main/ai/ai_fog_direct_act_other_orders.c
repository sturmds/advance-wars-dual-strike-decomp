#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020af5f8(void);
void AiUnit_PlanAttack(void);

/* AiUnit_ActByOrderFog, the queue routine of AiAction_QueueAllDirectUnitsFog,
 * runs this for units with any AI order but 1, 7 and 10. Same steps as
 * AiFogDirect_ActOrders1_10; AiUnit_PlanAttack picks the attack. */
void AiFogDirect_ActOtherOrders(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
}
