#include "types.h"
#include "game/sprite_anim.h"

int SpriteAnim_Advance(SpriteAnim *sprite);

/* Starts the animation over at its first frame; the speed in unk_1a (0x100
 * when new) is kept. unk_18 is cleared twice; both stores are needed to match. */
void SpriteAnim_Restart(SpriteAnim *sprite)
{
    u16 savedSpeed;

    sprite->unk_18 = 0;
    savedSpeed = sprite->unk_1a;
    sprite->unk_18 = 0;
    sprite->unk_1a = 0x100;
    SpriteAnim_Advance(sprite);
    sprite->unk_1a = savedSpeed;
}
