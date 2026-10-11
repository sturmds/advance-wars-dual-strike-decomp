#include "types.h"
#include "game/sprite_anim.h"

/* Points a slot at a sprite file. Word 0 of the file leads to the first
 * section; word 1 leads to the animation table, whose first entry becomes the
 * current animation. In a halfword file the links are halfword offsets from
 * the file start (or, for table entries, from the table start). The file is
 * reread through the slot each time, as the original does. */
void SpriteAnim_SetFile(SpriteAnim *sprite, const void *file)
{
    const u16 *half;
    const u16 *table;
    const void *const *words;

    sprite->data = file;
    if (sprite->flags & SPRITE_ANIM_HALFWORD_FILE) {
        half = sprite->data;
        sprite->unk_04 = half + (half[0] >> 1);
        half = sprite->data;
        table = half + (half[1] >> 1);
        sprite->animation = table + (table[0] >> 1);
        half = sprite->data;
        table = half + (half[1] >> 1);
        sprite->step = table + (table[0] >> 1);
    } else {
        words = sprite->data;
        sprite->unk_04 = words[0];
        sprite->animation = words[1];
        sprite->step = words[1];
    }
}
