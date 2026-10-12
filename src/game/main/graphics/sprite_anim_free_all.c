#include "types.h"
#include "game/sprite_anim.h"

/* Frees every SpriteAnim slot. */
void SpriteAnim_FreeAll(void) {
  int i;

  for (i = 0; i < SPRITE_ANIM_COUNT; i++) {
    gSpriteAnims[i].data = 0;
  }
}
