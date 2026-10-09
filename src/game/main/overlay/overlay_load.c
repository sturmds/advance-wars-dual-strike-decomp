#include "types.h"

extern u32 data_0216e020[7];   /* per slot: 1 while its overlay is loaded */
extern u32 data_02151ca0[7];   /* overlay id of each slot (0 to 6) */

int func_02112fa4(int target, u32 overlayId);   /* loads an overlay; 0 on failure */

/* Loads the overlay of this slot unless it is already loaded. */
void Overlay_Load(int slot)
{
    if (data_0216e020[slot] != 0) {
        return;
    }
    if (func_02112fa4(0, data_02151ca0[slot])) {
        data_0216e020[slot] = 1;
    }
}
