#include "types.h"

void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Runs func_01ff8c84 and queues the layer copy for layer 0, then for layer 1. */
void UnitInfoPanel_RequestLayerCopies(void)
{
    func_01ff8c84(0);
    Gfx_RequestLayerCopy(0);
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(1);
}
