#include "types.h"

void Gfx_RequestLayerCopy(int layer);

/* Callback for GfxLayer_GetCopyCallback: queues layer 2's tile map copy to VRAM. */
void GfxLayer2_RequestCopy(void) { Gfx_RequestLayerCopy(2); }
