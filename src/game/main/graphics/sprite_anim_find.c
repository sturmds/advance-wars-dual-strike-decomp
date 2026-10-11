#include "types.h"
#include "game/sprite_anim.h"

/* Returns the slot that holds file, or NULL. Passing 0 finds a free slot. */
SpriteAnim *SpriteAnim_Find(const void *file)
{
    int i;

    for (i = 0; i < SPRITE_ANIM_COUNT; i++) {
        if (data_0229b080[i].data == file) {
            return &data_0229b080[i];
        }
    }
    return NULL;
}
