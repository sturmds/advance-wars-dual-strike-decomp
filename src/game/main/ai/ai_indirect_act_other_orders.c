#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020af5f8(void);
void AiUnit_PlanAttack(void);

/* AiUnit_ActByOrderGroup runs this for indirect units with any AI order
 * but 1, 7 and 10. AiUnit_PlanAttack picks the attack. */
void AiIndirect_ActOtherOrders(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
}
