#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_UnpackResource(const char *name, void *dst);

/* Unpacks a 32x32 tile map block from the named resource and ORs flags into
 * each entry as it copies the block into the big map at (blockX, blockY).
 * With flag 0x400 (horizontal flip) each row is copied mirrored. */
void Ov004_BlitBlockWithFlags(Unk_ov004_022bf158 *state, int blockX, int blockY, const char *name, u16 flags) {
    int stride = state->blocksWide;
    u16 *block = state->mapBlock;
    u16 *map = state->tileMap;
    int origin;
    u16 *dst;
    int row;
    int col;
    u16 *srcRow;
    u16 *dstRow;

    stride <<= 5;
    origin = stride * (blockY << 5) + (blockX << 5);
    Ov004_UnpackResource(name, block);
    if (flags & 0x400) {
        dst = map + origin;
        for (row = 0; row < 32; row++) {
            srcRow = block + row * 32;
            dstRow = dst + row * stride;
            for (col = 0; col < 32; col++) {
                dstRow[31 - col] = srcRow[col] | flags;
            }
        }
    } else {
        dst = map + origin;
        for (row = 0; row < 32; row++) {
            srcRow = block + row * 32;
            dstRow = dst + row * stride;
            for (col = 0; col < 32; col++) {
                dstRow[col] = srcRow[col] | flags;
            }
        }
    }
}
