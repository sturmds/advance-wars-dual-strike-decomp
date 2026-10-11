#include "types.h"
#include "sdk/fx.h"

const void *func_02021a38(const void *file, int animation, int time);
void func_020146f0(const void *cells, const VecFx32 *pos, u32 tileBase, u32 unk28, int arg, int drawFlag);

/* Draws the frame of animation at time from a sprite file, without a
 * SpriteAnim slot. Returns FALSE when the animation has ended. The draw
 * sits inside `if (cells != NULL)` to match: an early return for NULL
 * moves a stack argument load. */
BOOL SpriteFile_Draw3d(const void *file, int animation, int time, const VecFx32 *pos, u32 tileBase, u32 unk28, int drawFlag)
{
    const void *cells = func_02021a38(file, animation, time);

    if (cells != NULL) {
        func_020146f0(cells, pos, tileBase, unk28, 0, drawFlag);
        return TRUE;
    }
    return FALSE;
}
