#include "types.h"

void Gfx_RequestLayerCopy(int layer);

/* Callback for GfxLayer_GetCopyCallback: queues layer 1's tile map copy to VRAM. */
void GfxLayer1_RequestCopy(void) { Gfx_RequestLayerCopy(1); }
