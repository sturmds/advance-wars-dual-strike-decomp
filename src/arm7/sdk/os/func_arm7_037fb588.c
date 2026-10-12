#include "sdk/os.h"

extern u16 data_arm7_0380773c[];   /* IE bit of each ARM7 IRQ slot */
OSIrqMask func_arm7_037fb654(OSIrqMask mask);

/*
 * OS7_IRQ_SLOTS and its views, as symbols. None of these exist yet: the
 * original's literals for them have no relocations because dsd puts the
 * start of .bss at 0x038078a0, past the table's first entries. With the
 * OS7_ constants the compiler splits each address into add instructions,
 * so this file stays unlinked until the relocations exist.
 */
extern IrqSlotEntry data_arm7_0380788c[];
extern IrqSlotKeepAlias data_arm7_03807890[];
extern IrqSlotArgAlias data_arm7_03807894[];

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
