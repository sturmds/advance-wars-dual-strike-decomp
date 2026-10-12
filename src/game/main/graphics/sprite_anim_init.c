#include "types.h"
#include "game/sprite_anim.h"

void SpriteAnim_SetFile(SpriteAnim* sprite, const void* file);
int SpriteAnim_Advance(SpriteAnim* sprite);

/* Sets up a slot to play file on layer, then runs its first frame
 * (SpriteAnim_Advance). framesLeft is cleared twice; the first store is needed to match. */
void SpriteAnim_Init(SpriteAnim* sprite, const void* file, u16 layer,
                     u8 flags) {
  sprite->flags = flags;
  SpriteAnim_SetFile(sprite, file);
  sprite->tileSource = NULL;
  sprite->tileBase = 0;
  sprite->unk_28 = 0;
  sprite->framesLeft = 0;
  sprite->layer = layer;
  sprite->affine = NULL;
  sprite->affineBase = 0;
  sprite->framesLeft = 0;
  sprite->speed = 0x100;
  sprite->speedFraction = 0;
  sprite->lastCommand = 0;
  SpriteAnim_Advance(sprite);
}
