#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_UnpackResource(const char* name, void* dst);

/* Unpacks a 32x32 tile map block from the named resource and ORs flags into
 * each entry as it copies the block into the big map at (blockX, blockY).
 * With flag 0x400 (horizontal flip) each row is copied mirrored. */
void Ov004_BlitBlockWithFlags(Unk_ov004_022bf158* state, int blockX, int blockY,
                              const char* name, u16 flags) {
  u16* map;
  u16* dst;
  u16* block;
  int stride;
  int col;
  int origin;

  /* Not matched yet (91.6%): in the flip branch dst shares map's register,
     * and the else branch's row steps swap registers. Both rows written as
     * (base + row * n)[i] give the original's index-first addresses. Needed
     * for the registers so far: the flip branch counts in its own block-scoped
     * i and j, and the else branch counts rows in the blockX parameter (that
     * puts flags in r2). */
  stride = state->blocksWide * 32;
  block = state->mapBlock;
  map = state->tileMap;
  origin = stride * (blockY << 5) + (blockX << 5);
  Ov004_UnpackResource(name, block);
  if (flags & 0x400) {
    int i;
    int j;

    dst = map + origin;
    for (i = 0; i < 32; i++) {
      for (j = 0; j < 32; j++) {
        (dst + i * stride)[31 - j] = (block + i * 32)[j] | flags;
      }
    }
  } else {
    map += origin;
    for (blockX = 0; blockX < 32; blockX++) {
      for (col = 0; col < 32; col++) {
        (map + blockX * stride)[col] = (block + blockX * 32)[col] | flags;
      }
    }
  }
}
