#include "types.h"

void Gfx_RequestLayerCopy(int layer);

/* Callback for GfxLayer_GetCopyCallback: queues layer 0's tile map copy to VRAM. */
void GfxLayer0_RequestCopy(void) { Gfx_RequestLayerCopy(0); }
