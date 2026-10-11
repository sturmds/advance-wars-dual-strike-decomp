#include "types.h"
#include "sdk/gx.h"

/* Stores the projection stack level (GXSTAT bit 13) in *level.
 * Returns -1 without storing when the stack has overflowed (bit 14). */
int G3X_GetMtxStackLevelPJ(int *level)
{
    if (*REG_GXSTAT & GXSTAT_MTX_STACK_ERROR) {
        return -1;
    }
    *level = (*REG_GXSTAT & GXSTAT_PJ_LEVEL) >> 13;
    return 0;
}
