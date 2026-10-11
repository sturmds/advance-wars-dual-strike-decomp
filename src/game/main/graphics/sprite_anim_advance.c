#include "types.h"
#include "game/sprite_anim.h"

void SpriteAnim_Free(SpriteAnim *sprite);

/* Advances a sprite by one frame. When the current step runs out it moves
 * to the next one, runs commands, and points cells (and affine) at the new
 * frame. Returns 0 for a free sprite or a finished animation, else 1.
 * Fields: unk_0c is the current step, unk_18 the frames left in it, unk_1a
 * the speed (0x100 = one frame per tick, 0 = paused), unk_1c the fraction
 * carried between steps, unk_22 the value of the last unknown command. */
int SpriteAnim_Advance(SpriteAnim *sprite)
{
    BOOL active;
    const SpriteAnimStep *step;
    u16 value;
    const u16 *table;
    s16 timer;

    if (sprite != NULL && sprite->data != 0) {
        active = TRUE;
    } else {
        active = FALSE;
    }
    if (!active) {
        return 0;
    }
    if (sprite->unk_1a == 0) {
        return 1;
    }
    timer = sprite->unk_18;
    if (timer != 0) {
        sprite->unk_18 = timer - 1;
        if ((s16)sprite->unk_18 > 0) { /* the u16 field is read as signed */
            return 1;
        }
        sprite->unk_18 = 0;
        sprite->unk_0c = (const SpriteAnimStep *)sprite->unk_0c + 1;
    }
    step = sprite->unk_0c;
    if (step->duration == 0) {
        value = step->value;
        switch (value) {
        case SPRITE_ANIM_CMD_LOOP:
            sprite->unk_0c = sprite->animation;
            return SpriteAnim_Advance(sprite);
        case SPRITE_ANIM_CMD_STOP:
            return 0;
        case SPRITE_ANIM_CMD_FREE:
            SpriteAnim_Free(sprite);
            return 0;
        default:
            sprite->unk_22 = value;
            sprite->unk_0c = (const SpriteAnimStep *)sprite->unk_0c + 1;
            return SpriteAnim_Advance(sprite);
        }
    }
    sprite->unk_1c = step->duration * sprite->unk_1a + sprite->unk_1c;
    if (sprite->unk_1c >= 0x100) {
        sprite->unk_18 = sprite->unk_1c >> 8;
        sprite->unk_1c &= 0xff;
    } else {
        sprite->unk_18 = 1;
        return SpriteAnim_Advance(sprite);
    }
    if (sprite->flags & SPRITE_ANIM_HALFWORD_FILE) {
        table = sprite->unk_04;
        sprite->cells = table + (table[((const SpriteAnimStep *)sprite->unk_0c)->value] >> 1);
    } else {
        sprite->cells = ((const void *const *)sprite->unk_04)[((const SpriteAnimStep *)sprite->unk_0c)->value];
    }
    if (*(const u16 *)sprite->cells & SPRITE_ANIM_AFFINE_CELLS) {
        sprite->affine = sprite->cells;
        sprite->cells = (const u16 *)sprite->cells + ((*(const u16 *)sprite->affine & 0x7fff) * 3 + 1);
    } else {
        sprite->affine = NULL;
    }
    sprite->flags |= SPRITE_ANIM_TILES_PENDING;
    return 1;
}
