#include "types.h"
#include "game/sprite_anim.h"

void func_020146f0(const void *cells, const VecFx32 *pos, u32 tileBase, u32 unk28, int arg, int drawFlag);
void func_02021de8(SpriteAnim *sprite);
int func_020224d4(void);
int func_020224ec(void);

void func_02022504(SpriteAnim *sprite, const VecFx32 *pos, int arg)
{
    BOOL active;
    int skip;
    int alt;

    if (sprite != NULL && sprite->data != 0) {
        active = TRUE;
    } else {
        active = FALSE;
    }
    if (!active) {
        return;
    }
    skip = sprite->flags & SPRITE_ANIM_SKIP_DRAW;
    alt = sprite->flags & SPRITE_ANIM_ALT_REGISTER;
    if (skip) {
        sprite->flags &= ~SPRITE_ANIM_SKIP_DRAW;
    } else {
        if (alt) {
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
