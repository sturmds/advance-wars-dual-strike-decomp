#include "types.h"
#include "game/sprite_anim.h"

void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);
void SpriteAnim_UploadTiles(SpriteAnim* sprite);
void SpriteAnim_LoadAffineParams(SpriteAnim* sprite);

/* Queues the sprite's current cells at (x, y) on its layer. With affine
 * data the affine slot goes into the x word from bit 9 up. Uploads the
 * tiles first if they are still pending. Does nothing for a free slot. */
void SpriteAnim_Queue(SpriteAnim* sprite, int x, int y) {
  BOOL active;
  int affineBits = 0;

  if (sprite != NULL && sprite->data != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (!active) {
    return;
  }
  if (sprite->affine != NULL) {
    SpriteAnim_LoadAffineParams(sprite);
    affineBits = sprite->affineBase << 9;
  }
  Sprite_Queue(sprite->layer, x | affineBits, y, sprite->cells,
               sprite->tileBase, 0);
  if (sprite->tileSource != NULL &&
      (sprite->flags & SPRITE_ANIM_TILES_PENDING)) {
    SpriteAnim_UploadTiles(sprite);
  }
}
