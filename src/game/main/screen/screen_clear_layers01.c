#include "types.h"

void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);

void Screen_ClearLayers01(void)
{
    func_01ff8c84(0);
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
}
