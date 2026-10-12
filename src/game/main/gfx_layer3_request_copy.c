#include "types.h"

void Gfx_RequestLayerCopy(int layer);

/* Callback for GfxLayer_GetCopyCallback: queues layer 3's tile map copy to VRAM. */
void GfxLayer3_RequestCopy(void) { Gfx_RequestLayerCopy(3); }
