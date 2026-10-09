#include "types.h"

void func_0209ca48(void);
void AiUnit_PlanAttack(void);

/* AiUnit_ActByOrderGroup runs this for indirect units with AI order 1, 7
 * or 10. func_0209ca48 clears the per-tile AI maps (MapTiles.threatMax and
 * threatSum); AiUnit_PlanAttack picks the attack. */
void AiIndirect_ActOrders1_7_10(void)
{
    func_0209ca48();
    AiUnit_PlanAttack();
}
