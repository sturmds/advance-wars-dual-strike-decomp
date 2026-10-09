#include "types.h"

void func_0209ca48(void);
void func_020b1148(void);

/* AiUnit_ActByOrderFog, the queue routine of AiAction_QueueAllDirectUnitsFog,
 * runs this for units with AI order 7. func_0209ca48 clears the per-tile
 * AI maps (MapTiles.threatMax and threatSum). */
void AiFogDirect_ActOrder7(void)
{
    func_0209ca48();
    func_020b1148();
}
