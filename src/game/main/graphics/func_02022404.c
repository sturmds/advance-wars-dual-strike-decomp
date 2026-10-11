#include "types.h"

void SpriteAnim_Queue(u32 *handle, int x, int y);
int func_02022140(u32 *handle);

/* Positions a sprite handle at (x, y) and then runs func_02022140 on it
 * (meaning unknown) and returns its result. Returns 0 for a null or cleared handle. The flag
 * test is a separate step, as in func_0202244c. */
int func_02022404(u32 *handle, int x, int y)
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
    SpriteAnim_Queue(handle, x, y);
    return func_02022140(handle);
}
