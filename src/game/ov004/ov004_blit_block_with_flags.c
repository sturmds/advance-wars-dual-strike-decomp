#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_UnpackResource(const char *name, void *dst);

/* Unpacks a 32x32 tile map block from the named resource and ORs flags into
 * each entry as it copies the block into the big map at (blockX, blockY).
 * With flag 0x400 (horizontal flip) each row is copied mirrored. */
void Ov004_BlitBlockWithFlags(Unk_ov004_022bf158 *state, int blockX, int blockY, const char *name, u16 flags) {
    u16 *dstRow;
    u16 *block;
    u16 *dst;
    int stride;
    int origin;
    u16 *map;
    int col;
    int row;

    /* Not matched yet (89.0%): registers differ from the start (orig keeps
     * stride in r5, map in r7, flags in r2). The source row read as
     * (block + row * 32)[col] gives the original index-first address. */
    stride = state->blocksWide;
    block = state->mapBlock;
    map = state->tileMap;
    stride <<= 5;
    origin = stride * (blockY << 5) + (blockX << 5);
    Ov004_UnpackResource(name, block);
    if (flags & 0x400) {
        dst = map + origin;
        for (row = 0; row < 32; row++) {
            dstRow = dst + row * stride;
            for (col = 0; col < 32; col++) {
                dstRow[31 - col] = (block + row * 32)[col] | flags;
            }
        }
    } else {
        dst = map + origin;
        for (row = 0; row < 32; row++) {
            dstRow = dst + row * stride;
            for (col = 0; col < 32; col++) {
                dstRow[col] = (block + row * 32)[col] | flags;
            }
        }
    }
}
