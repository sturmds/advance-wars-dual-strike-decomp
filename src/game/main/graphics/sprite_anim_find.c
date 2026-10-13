#include "types.h"
#include "game/sprite_anim.h"

SpriteAnim gSpriteAnims[SPRITE_ANIM_COUNT];

/* Returns the slot that holds file, or NULL. Passing 0 finds a free slot. */
SpriteAnim* SpriteAnim_Find(const void* file) {
  int i;

  for (i = 0; i < SPRITE_ANIM_COUNT; i++) {
    if (gSpriteAnims[i].data == file) {
      return &gSpriteAnims[i];
    }
  }
  return NULL;
}
