#include "types.h"

/* Maps a kind 0-3 to 0, 1, 4 or 5; anything else gives 0. Name kept: the
 * callers (func_020d92fc and func_020d9494) are not decompiled yet. */
int func_020d92c0(u32 kind)
{
    switch (kind) {
    case 0:
    default:
        return 0;
    case 1:
        return 1;
    case 2:
        return 4;
    case 3:
        return 5;
    }
}
