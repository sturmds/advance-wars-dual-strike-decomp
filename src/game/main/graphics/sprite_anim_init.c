#include "types.h"
#include "game/sprite_anim.h"

void SpriteAnim_SetFile(SpriteAnim *sprite, const void *file);
int func_02022140(SpriteAnim *sprite);

/* Sets up a slot to play file on layer, then runs its first frame
 * (func_02022140). unk_18 is cleared twice; the first store is needed to match. */
void SpriteAnim_Init(SpriteAnim *sprite, const void *file, u16 layer, u8 flags)
{
    sprite->flags = flags;
    SpriteAnim_SetFile(sprite, file);
    sprite->tileSource = NULL;
    sprite->tileBase = 0;
    sprite->unk_28 = 0;
    sprite->unk_18 = 0;
    sprite->layer = layer;
    sprite->affine = NULL;
    sprite->affineBase = 0;
    sprite->unk_18 = 0;
    sprite->unk_1a = 0x100;
    sprite->unk_1c = 0;
    sprite->unk_22 = 0;
    func_02022140(sprite);
}
