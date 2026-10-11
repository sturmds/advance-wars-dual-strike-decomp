#include "types.h"

/* Zeroes a saved list of four skill ids (the u8 form Skills_SetFromList reads); NULL is ignored. */
void ForceRank_ClearSet(u8 *set)
{
    int i;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        set[i] = 0;
    }
}
