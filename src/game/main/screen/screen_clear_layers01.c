#include "types.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

void Screen_ClearLayers01(void) {
  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
}
