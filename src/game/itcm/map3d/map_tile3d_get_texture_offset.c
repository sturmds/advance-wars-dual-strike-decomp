#include "types.h"

int func_01ffc5b8(int tile);

/* Texture offset of map tile image number tile. The first 79 images take
 * 256 bytes each and the later ones 128 bytes, from offset 0x4f00 (the
 * size comes from func_01ffc5b8). */
int MapTile3d_GetTextureOffset(int tile)
{
    if (tile < 79) {
        return (tile << 7) * func_01ffc5b8(tile);
    }
    return ((tile - 79) << 7) * func_01ffc5b8(tile) + 0x4f00;
}
