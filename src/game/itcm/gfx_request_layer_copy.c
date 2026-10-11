#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

/* Marks a layer's tile map to be copied to VRAM by Gfx_CopyRequestedLayers. */
void Gfx_RequestLayerCopy(int layer)
{
    Unk_027c0280 *gfx = data_027c0280;

    /* needed to match: a one-case switch keeps the early return a branch */
    switch (GFX_LAYER(gfx, layer)->mode) {
    case GFX_LAYER_NO_COPY:
        return;
    }
    gfx->layerCopyMask |= 1 << layer;
}
