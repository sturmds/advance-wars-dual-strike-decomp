#include "types.h"

u16 *func_020eac50(int mode);

/* Number of maps in the run for the mode variant: the entries of its map
 * list before the 0 that ends it. */
int Survival_GetMapCount(int mode)
{
    u16 *maps = func_020eac50(mode);
    int count = 0;

    while (*maps != 0) {
        count++;
        maps++;
    }
    return count;
}
