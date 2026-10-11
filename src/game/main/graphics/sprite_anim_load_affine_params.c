#include "types.h"
#include "game/sprite_anim.h"

extern s16 gSineTable[]; /* sine table, index 64 = cosine of 0 */

s32 HwDiv32(s32 numerator, s32 denominator);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);

#define AFFINE_COUNT_MASK 0x7fff
#define SIN(angle) gSineTable[(angle) & 0xff]
#define COS(angle) gSineTable[((angle) & 0xff) + 64]

/* Sets the OAM affine matrices of the sprite's current frame, from affineBase on: each
 * SpriteAffineEntry gives a rotation and a scale. Does nothing for a free slot or a frame
 * without affine data. */
void SpriteAnim_LoadAffineParams(SpriteAnim *sprite)
{
    BOOL active;
    const u16 *data;
    int i;
    const SpriteAffineEntry *entry;
    int count;

    if (sprite != NULL && sprite->data != 0) {
        active = TRUE;
    } else {
        active = FALSE;
    }
    if (!active) {
        return;
    }
    data = sprite->affine;
    if (data == NULL) {
        return;
    }
    entry = (const SpriteAffineEntry *)(data + 1);
    count = data[0] & AFFINE_COUNT_MASK;
    for (i = 0; i < count; i++) {
        Oam_SetAffineParams(sprite->affineBase + i,
                            HwDiv32(COS(entry->angle) << 4, entry->scaleX),
                            HwDiv32(-SIN(entry->angle) << 4, entry->scaleY),
                            HwDiv32(SIN(entry->angle) << 4, entry->scaleX),
                            HwDiv32(COS(entry->angle) << 4, entry->scaleY));
        entry++;
    }
}
