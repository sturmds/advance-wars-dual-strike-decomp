#include "sdk/os.h"

/* Handlers for the interrupts that have no slot in gOsIrqSlots, indexed by IE bit. */
extern OSIrqFunction data_027c127c[];

void OS_SetIrqFunction(OSIrqMask mask, OSIrqFunction function)
{
    int bit;

    for (bit = 0; bit < 22; bit++, mask >>= 1) {
        IrqSlotEntry *slot;

        if ((mask & 1) == 0) {
            continue;
        }
        slot = NULL;
        if (bit >= 8 && bit <= 11) {
            slot = &gOsIrqSlots[bit - 8];
        } else if (bit >= 3 && bit <= 6) {
            slot = &gOsIrqSlots[bit + 1];
        } else {
            data_027c127c[bit] = function;
        }
        if (slot != NULL) {
            /* slot handlers take an argument, which is NULL here */
            slot->handler = (void (*)(void *))function;
            slot->arg = NULL;
            slot->keepEnabled = TRUE;
        }
    }
}
