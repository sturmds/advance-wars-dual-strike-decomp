#include "types.h"

void SpriteAnim_Queue3d(u32 *handle, int x, int y);
int SpriteAnim_Advance(u32 *handle);

int func_020225c8(u32 *handle, int x, int y)
{
    BOOL active;

    if (handle != NULL && *handle != 0) {
        active = TRUE;
    } else {
        active = FALSE;
    }
    if (!active) {
        return 0;
    }
    SpriteAnim_Queue3d(handle, x, y);
    return SpriteAnim_Advance(handle);
}
