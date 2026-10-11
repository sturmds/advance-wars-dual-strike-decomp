#include "types.h"

int MapTile3d_GetImageScale(int tile);

/* Texture offset of map tile image number tile. The first 79 images take
 * 256 bytes each and the later ones 128 bytes, from offset 0x4f00 (the
 * size comes from MapTile3d_GetImageScale). */
int MapTile3d_GetTextureOffset(int tile)
{
    if (tile < 79) {
        return (tile << 7) * MapTile3d_GetImageScale(tile);
    }
    return ((tile - 79) << 7) * MapTile3d_GetImageScale(tile) + 0x4f00;
}
