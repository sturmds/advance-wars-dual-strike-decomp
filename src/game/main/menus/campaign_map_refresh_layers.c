#include "types.h"

BOOL func_0204d5e0(void);
void CampaignMap_CopyLayer1SecondPage(void);
void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Redraws layers 0 and 1. With the world map task running (func_0204d5e0)
 * it also copies layer 1's second tile map page first. */
void CampaignMap_RefreshLayers(void)
{
    if (func_0204d5e0() == 1) {
        CampaignMap_CopyLayer1SecondPage();
    } else {
        func_01ff8c84(0);
        func_01ff8c84(1);
        Gfx_RequestLayerCopy(0);
        Gfx_RequestLayerCopy(1);
    }
}
