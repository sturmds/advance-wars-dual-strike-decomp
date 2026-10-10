#include "types.h"
#include "game/Unk_ov003_02369bb8.h"

u32 func_020279c4(int x1, int y1, int x2, int y2);

/* x and y are in pixels. Kind 6 reaches 8 pixels, kind 7 reaches 24. */
int Ov003_IsEntryNear(Unk_ov003_02369bb8 *entry, int x, int y)
{
    u32 radius;

    switch (entry->unk_08_0) {
    case 6:
        radius = 8;
        break;
    case 7:
        radius = 24;
        break;
    default:
        return 0;
    }
    return func_020279c4(x, y, (entry->x * 128 + 64) >> 3, (entry->y * 128 + 64) >> 3) <= radius;
}
