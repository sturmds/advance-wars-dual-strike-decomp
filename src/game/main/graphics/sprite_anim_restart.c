#include "types.h"
#include "game/sprite_anim.h"

int SpriteAnim_Advance(SpriteAnim* sprite);

/* Starts the animation over at its first frame; the speed (0x100
 * when new) is kept. framesLeft is cleared twice; both stores are needed to match. */
void SpriteAnim_Restart(SpriteAnim* sprite) {
  u16 savedSpeed;

  sprite->framesLeft = 0;
  savedSpeed = sprite->speed;
  sprite->framesLeft = 0;
  sprite->speed = 0x100;
  SpriteAnim_Advance(sprite);
  sprite->speed = savedSpeed;
}
