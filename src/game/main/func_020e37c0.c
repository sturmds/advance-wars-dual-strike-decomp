#include "types.h"

void Gfx_ClearLayerMap(int);
void Gfx_RequestLayerCopy(int layer);

void func_020e37c0(void)
{
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
}
