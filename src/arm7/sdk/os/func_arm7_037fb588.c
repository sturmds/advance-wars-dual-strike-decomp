#include "sdk/os_arm7.h"

extern u16 data_arm7_0380773c[];   /* IE bit of each ARM7 IRQ slot */
OSIrqMask func_arm7_037fb654(OSIrqMask mask);

/*
 * ARM7 copy of IrqSlot_CallAndClear, the shared body of the DMA and timer interrupt
 * handlers: runs the slot's handler once, sets the IRQ check flag and
 * disables the interrupt unless the slot asks to stay enabled.
 */
void func_arm7_037fb588(int slot)
{
    OSIrqMask mask;
    void (*handler)(void *);

    mask = 1 << data_arm7_0380773c[slot];
    handler = data_arm7_0380788c[slot].handler;
    data_arm7_0380788c[slot].handler = NULL;
    if (handler != NULL) {
        handler(data_arm7_03807894[slot].arg);
    }
    OS7_IRQ_CHECK_FLAGS |= mask;
    if (data_arm7_03807890[slot].keepEnabled == 0) {
        func_arm7_037fb654(mask);
    }
}
