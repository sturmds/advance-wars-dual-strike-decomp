#include "types.h"

void Gfx_RequestLayerCopy(int layer);

void GfxLayer0_RequestCopy(void) {
    Gfx_RequestLayerCopy(0);
}
