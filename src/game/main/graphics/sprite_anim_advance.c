#include "types.h"
#include "game/sprite_anim.h"

void SpriteAnim_Free(SpriteAnim* sprite);

/* Advances a sprite by one frame. When the current step runs out it moves
 * to the next one, runs commands, and points cells (and affine) at the new
 * frame. Returns 0 for a free sprite or a finished animation, else 1. */
int SpriteAnim_Advance(SpriteAnim* sprite) {
  BOOL active;
  const SpriteAnimStep* step;
  u16 value;
  const u16* table;
  s16 timer;

  if (sprite != NULL && sprite->data != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (!active) {
    return 0;
  }
  if (sprite->speed == 0) {
    return 1;
  }
  timer = sprite->framesLeft;
  if (timer != 0) {
    sprite->framesLeft = timer - 1;
    if (sprite->framesLeft > 0) {
      return 1;
    }
    sprite->framesLeft = 0;
    sprite->step = (const SpriteAnimStep*)sprite->step + 1;
  }
  step = sprite->step;
  if (step->duration == 0) {
    value = step->value;
    switch (value) {
      case SPRITE_ANIM_CMD_LOOP:
        sprite->step = sprite->animation;
        return SpriteAnim_Advance(sprite);
      case SPRITE_ANIM_CMD_STOP:
        return 0;
      case SPRITE_ANIM_CMD_FREE:
        SpriteAnim_Free(sprite);
        return 0;
      default:
        sprite->lastCommand = value;
        sprite->step = (const SpriteAnimStep*)sprite->step + 1;
        return SpriteAnim_Advance(sprite);
    }
  }
  sprite->speedFraction =
      step->duration * sprite->speed + sprite->speedFraction;
  if (sprite->speedFraction >= 0x100) {
    sprite->framesLeft = sprite->speedFraction >> 8;
    sprite->speedFraction &= 0xff;
  } else {
    sprite->framesLeft = 1;
    return SpriteAnim_Advance(sprite);
  }
  if (sprite->flags & SPRITE_ANIM_HALFWORD_FILE) {
    table = sprite->unk_04;
    sprite->cells =
        table + (table[((const SpriteAnimStep*)sprite->step)->value] >> 1);
  } else {
    sprite->cells =
        ((const void* const*)
             sprite->unk_04)[((const SpriteAnimStep*)sprite->step)->value];
  }
  if (*(const u16*)sprite->cells & SPRITE_ANIM_AFFINE_CELLS) {
    sprite->affine = sprite->cells;
    sprite->cells = (const u16*)sprite->cells +
                    ((*(const u16*)sprite->affine & 0x7fff) * 3 + 1);
  } else {
    sprite->affine = NULL;
  }
  sprite->flags |= SPRITE_ANIM_TILES_PENDING;
  return 1;
}
