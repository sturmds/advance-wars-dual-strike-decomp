#include "types.h"

void Gfx_ClearLayerMap(int);
void Gfx_RequestLayerCopy(int layer);

void func_020bbe54(void) {
  Gfx_ClearLayerMap(0);
  Gfx_RequestLayerCopy(0);
}
