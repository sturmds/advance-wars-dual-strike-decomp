#include "types.h"

BOOL func_0204d5e0(void);
void CampaignMap_CopyLayer1SecondPage(void);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Redraws layers 0 and 1. With the world map task running (func_0204d5e0)
 * it also copies layer 1's second tile map page first. */
void CampaignMap_RefreshLayers(void) {
  if (func_0204d5e0() == 1) {
    CampaignMap_CopyLayer1SecondPage();
  } else {
    Gfx_ClearLayerMap(0);
    Gfx_ClearLayerMap(1);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
  }
}
