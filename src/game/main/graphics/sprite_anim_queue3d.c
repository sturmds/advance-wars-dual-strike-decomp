/* Needed to match: with common subexpression removal on, the flags byte
 * lands in r1 instead of r0. */
#pragma opt_common_subs off

#include "types.h"
#include "game/sprite_anim.h"

void SpriteCells_Draw3d(const void* cells, const VecFx32* pos, u32 tileBase,
                        u32 unk28, int arg, int drawFlag);
void SpriteAnim_UploadTiles(SpriteAnim* sprite);
int func_020224d4(void);
int func_020224ec(void);

/* Draws the sprite's current cells as 3D quads at *pos, the 3D counterpart of
 * SpriteAnim_Queue. SPRITE_ANIM_SKIP_DRAW skips one draw. */
void SpriteAnim_Queue3d(SpriteAnim* sprite, const VecFx32* pos, int arg) {
  BOOL active;

  if (sprite != NULL && sprite->data != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (!active) {
    return;
  }
  if (sprite->flags & SPRITE_ANIM_SKIP_DRAW) {
    sprite->flags &= ~SPRITE_ANIM_SKIP_DRAW;
  } else {
    if (sprite->flags & SPRITE_ANIM_ALT_REGISTER) {
      func_020224ec();
    } else {
      func_020224d4();
    }
    SpriteCells_Draw3d(sprite->cells, pos, sprite->tileBase, sprite->unk_28,
                       arg, sprite->flags & SPRITE_ANIM_DRAW_FLAG);
  }
  if (sprite->tileSource != NULL &&
      (sprite->flags & SPRITE_ANIM_TILES_PENDING)) {
    SpriteAnim_UploadTiles(sprite);
  }
}
