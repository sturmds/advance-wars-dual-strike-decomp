#include "types.h"
#include "game/sprite_anim.h"

SpriteAnim* SpriteAnim_Find(const void* file);
void SpriteAnim_Init(SpriteAnim* sprite, const void* file, u16 layer, u8 flags);

/* Takes a free slot and starts it on file. flags are SPRITE_ANIM_* bits. */
SpriteAnim* SpriteAnim_Create(const void* file, u16 layer, u8 flags) {
  SpriteAnim* sprite = SpriteAnim_Find(NULL);

  SpriteAnim_Init(sprite, file, layer, flags);
  return sprite;
}
