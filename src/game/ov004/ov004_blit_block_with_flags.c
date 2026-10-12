#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_UnpackResource(const char *name, void *dst);

/* Unpacks a 32x32 tile map block from the named resource and ORs flags into
 * each entry as it copies the block into the big map at (blockX, blockY).
 * With flag 0x400 (horizontal flip) each row is copied mirrored. */
void Ov004_BlitBlockWithFlags(Unk_ov004_022bf158 *state, int blockX, int blockY, const char *name, u16 flags) {
    u16 *map;
    u16 *dst;
    u16 *block;
    int stride;
    int col;
    int row;
    int origin;

    /* Not matched yet (90.4%): flags lands in r1 where the original has r2,
     * and in the flip branch dst shares map's register. Both rows written as
     * (base + row * n)[i] give the original's index-first addresses. */
    stride = state->blocksWide * 32;
    block = state->mapBlock;
    map = state->tileMap;
    origin = stride * (blockY << 5) + (blockX << 5);
    Ov004_UnpackResource(name, block);
    if (flags & 0x400) {
        dst = map + origin;
        for (row = 0; row < 32; row++) {
            for (col = 0; col < 32; col++) {
                (dst + row * stride)[31 - col] = (block + row * 32)[col] | flags;
            }
        }
    } else {
        map += origin;
        for (row = 0; row < 32; row++) {
            for (col = 0; col < 32; col++) {
                (map + row * stride)[col] = (block + row * 32)[col] | flags;
            }
        }
    }
}
