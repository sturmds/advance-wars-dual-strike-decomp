#include "types.h"
#include "game/sprite_anim.h"

void SpriteAnim_Restart(SpriteAnim* sprite);

/* Selects entry animation of the file's animation table and restarts it.
 * Does nothing for a free slot. Needed to match: the byte offset is added
 * to the table address as an int, offset first. */
void SpriteAnim_SetAnimation(SpriteAnim* sprite, int animation) {
  BOOL active;
  int offset;
  const u16* half;
  const u16* table;
  const void* const* words;

  if (sprite != NULL && sprite->data != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (!active) {
    return;
  }
  if (sprite->flags & SPRITE_ANIM_HALFWORD_FILE) {
    offset = animation * 2;
    half = sprite->data;
    table = half + (half[1] >> 1);
    sprite->animation = table + (*(const u16*)(offset + (int)table) >> 1);
    half = sprite->data;
    table = half + (half[1] >> 1);
    sprite->step = table + (*(const u16*)(offset + (int)table) >> 1);
  } else {
    words = sprite->data;
    sprite->animation = words[animation + 1];
    sprite->step = words[animation + 1];
  }
  SpriteAnim_Restart(sprite);
}
