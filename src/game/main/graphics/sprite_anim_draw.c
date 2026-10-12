#include "types.h"
#include "game/sprite_anim.h"

void SpriteAnim_Queue(SpriteAnim* sprite, int x, int y);
int SpriteAnim_Advance(SpriteAnim* sprite);

/* Queues a sprite at (x, y), then runs SpriteAnim_Advance on it and returns
 * its result. Returns 0 for a null or free sprite. The flag test is a
 * separate step, as in SpriteAnim_Free. */
int SpriteAnim_Draw(SpriteAnim* sprite, int x, int y) {
  BOOL active;

  if (sprite != NULL && sprite->data != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (!active) {
    return 0;
  }
  SpriteAnim_Queue(sprite, x, y);
  return SpriteAnim_Advance(sprite);
}
