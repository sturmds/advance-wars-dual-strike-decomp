#include "types.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Runs Gfx_ClearLayerMap and queues the layer copy for layer 0, then for layer 1. */
void UnitInfoPanel_RequestLayerCopies(void)
{
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
    Gfx_ClearLayerMap(1);
    Gfx_RequestLayerCopy(1);
}
