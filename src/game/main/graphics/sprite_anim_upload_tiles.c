#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/sprite_anim.h"

extern Unk_027c0280* data_027c0280;
extern u8
    gObjSizeInTiles[]; /* width and height in tiles, by OAM shape * 4 + size */

void GfxQueue_AddKind1(const void* src, void* dst, u32 size);
void TransferQueue_Add(const void* src, void* dst, u32 size);

#define TILE_BYTES_SHIFT 5 /* 32 bytes per 4bpp tile */
#define OBJ_TILE_NUMBER_MASK 0x3ff

/* Index into gObjSizeInTiles: shape from attribute 0 bits 14-15, size from attribute 1 bits 14-15. */
#define OBJ_SIZE_INDEX(oam) \
  ((((oam)[0] & 0xc000) >> 12) + (((oam)[1] & 0xc000) >> 14))

static inline int Obj_GetTileCount(const u16* oam) {
  return gObjSizeInTiles[OBJ_SIZE_INDEX(oam) * 2] *
         gObjSizeInTiles[OBJ_SIZE_INDEX(oam) * 2 + 1];
}

/* Copies the tiles of the sprite's current cells from tileSource to OBJ VRAM, one block per
 * cell, back to back from the sprite's tile number, then clears SPRITE_ANIM_TILES_PENDING.
 * The cell data holds a count, count OAM entries of 3 halfwords, then one source tile
 * number per cell. Only copies while OBJ tiles use 1D mapping. With
 * SPRITE_ANIM_TILE_DIRECT the copy goes through GfxQueue_AddKind1 to tileBase as an
 * address, and the byte offset is shifted by 5 again as if it counted tiles. */
void SpriteAnim_UploadTiles(SpriteAnim* sprite) {
  int offset;
  BOOL active;
  int count;
  int size;
  const u8* src;
  const u16* oam;
  const u16* tileIndex;

  if (sprite != NULL && sprite->data != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (!active) {
    return;
  }
  count = *(const u16*)sprite->cells;
  oam = (const u16*)sprite->cells + 1;
  offset = 0;
  tileIndex = oam + count * 3;
  while (count-- > 0) {
    if (GFX_DISPCNT(data_027c0280)->objMap1D) {
      src = (u8*)sprite->tileSource + (*tileIndex << TILE_BYTES_SHIFT);
      size = Obj_GetTileCount(oam) << TILE_BYTES_SHIFT;
      if (sprite->flags & SPRITE_ANIM_TILE_DIRECT) {
        GfxQueue_AddKind1(
            src, (u8*)sprite->tileBase + (offset << TILE_BYTES_SHIFT), size);
      } else {
        TransferQueue_Add(
            src,
            data_027c0280->objVram +
                ((sprite->tileBase & OBJ_TILE_NUMBER_MASK)
                 << TILE_BYTES_SHIFT) +
                offset,
            (u16)size); /* needed to match: the original narrows here */
      }
      offset += Obj_GetTileCount(oam) << TILE_BYTES_SHIFT;
    }
    oam += 3;
    tileIndex++;
  }
  sprite->flags &= ~SPRITE_ANIM_TILES_PENDING;
}
