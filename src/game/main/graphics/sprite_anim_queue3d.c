#include "types.h"
#include "game/sprite_anim.h"

void func_020146f0(const void *cells, const VecFx32 *pos, u32 tileBase, u32 unk28, int arg, int drawFlag);
void func_02021de8(SpriteAnim *sprite);
int func_020224d4(void);
int func_020224ec(void);

/* Draws the sprite's current cells as 3D quads at *pos, the 3D counterpart of
 * SpriteAnim_Queue. SPRITE_ANIM_SKIP_DRAW skips one draw. Not matched: the
 * flags byte lands in r1 instead of r0. */
void SpriteAnim_Queue3d(SpriteAnim *sprite, const VecFx32 *pos, int arg)
{
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
        func_020146f0(sprite->cells, pos, sprite->tileBase, sprite->unk_28, arg,
                      sprite->flags & SPRITE_ANIM_DRAW_FLAG);
    }
    if (sprite->tileSource != NULL && (sprite->flags & SPRITE_ANIM_TILES_PENDING)) {
        func_02021de8(sprite);
    }
}
