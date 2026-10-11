#include "types.h"

extern u32 data_0216e020[7];   /* per slot: 1 while its overlay is loaded */

/* Marks every overlay slot as not loaded. */
void Overlay_ClearLoadedFlags(void)
{
    int slot;

    for (slot = 0; slot < 7; slot++) {
        data_0216e020[slot] = 0;
    }
}
