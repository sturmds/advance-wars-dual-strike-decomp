#include "types.h"
#include "game/sprite_anim.h"

void SpriteAnim_Queue3d(SpriteAnim* sprite, const VecFx32* pos, int arg);
int SpriteAnim_Advance(SpriteAnim* sprite);

/* 3D counterpart of SpriteAnim_Draw: SpriteAnim_Queue3d, then
 * SpriteAnim_Advance. Returns 0 for a null or free sprite. */
int SpriteAnim_Draw3d(SpriteAnim* sprite, const VecFx32* pos, int arg) {
  BOOL active;

  if (sprite != NULL && sprite->data != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (!active) {
    return 0;
  }
  SpriteAnim_Queue3d(sprite, pos, arg);
  return SpriteAnim_Advance(sprite);
}
