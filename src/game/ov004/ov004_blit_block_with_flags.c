#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/map_bg.h"

/* Needed to match: with the row addresses written as (map + origin) + ...,
 * opt_loop_invariants on leaves them in the inner loop; off, mwcc moves them
 * out of the loops in the original's order. */
#pragma opt_loop_invariants off

void Ov004_UnpackResource(const char* name, void* dst);

/* Unpacks a 32x32 tile map block from the named resource and ORs flags into
 * each entry as it copies the block into the big map at (blockX, blockY).
 * With BG_TILE_HFLIP in flags each row is copied mirrored. */
void Ov004_BlitBlockWithFlags(Unk_ov004_022bf158* state, int blockX, int blockY,
                              const char* name, u16 flags) {
  u16* map;
  u16* block;
  int stride;
  int row;
  int col;
  int origin;

  stride = state->blocksWide * OV004_BLOCK_TILES;
  block = state->mapBlock;
  map = state->tileMap;
  origin = stride * (blockY << 5) + (blockX << 5);
  Ov004_UnpackResource(name, block);
  if (flags & BG_TILE_HFLIP) {
    for (row = 0; row < OV004_BLOCK_TILES; row++) {
      for (col = 0; col < OV004_BLOCK_TILES; col++) {
        ((map + origin) + row * stride)[OV004_BLOCK_TILES - 1 - col] =
            (block + row * OV004_BLOCK_TILES)[col] | flags;
      }
    }
  } else {
    for (row = 0; row < OV004_BLOCK_TILES; row++) {
      for (col = 0; col < OV004_BLOCK_TILES; col++) {
        ((map + origin) + row * stride)[col] =
            (block + row * OV004_BLOCK_TILES)[col] | flags;
      }
    }
  }
}
