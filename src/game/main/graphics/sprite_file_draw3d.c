#include "types.h"
#include "sdk/fx.h"

const void *func_02021a38(const void *file, int animation, int time);
void func_020146f0(const void *cells, const VecFx32 *pos, u32 tileBase, u32 unk28, int arg, int drawFlag);

/* Draws the frame of animation at time from a sprite file, without a
 * SpriteAnim slot. Returns FALSE when the animation has ended. Not matched:
 * the compiler loads the last stack argument before storing the 0 argument. */
BOOL SpriteFile_Draw3d(const void *file, int animation, int time, const VecFx32 *pos, u32 tileBase, u32 unk28, int drawFlag)
{
    const void *cells = func_02021a38(file, animation, time);

    if (cells == NULL) {
        return FALSE;
    }
    func_020146f0(cells, pos, tileBase, unk28, 0, drawFlag);
    return TRUE;
}
