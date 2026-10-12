#include "sdk/os.h"

extern OSIrqFunction data_arm7_03807750[];   /* handlers for the interrupts that have no slot, indexed by IE bit */
extern IrqSlotEntry data_arm7_038078ec;          /* OS7_IRQ_SLOTS[OS7_IRQ_SLOT_VBLANK] */

/*
 * OS7_IRQ_SLOTS as a symbol, which does not exist yet: the original's literal
 * for it has no relocation because dsd starts ARM7 .bss at 0x038078a0, past
 * the table's first entries. With the OS7_IRQ_SLOTS constant the compiler
 * loads it later and picks other registers, so this file stays unlinked
 * until the relocation exists.
 */
extern IrqSlotEntry data_arm7_0380788c[];

/* ARM7 copy of OS_SetIrqFunction. Unlike ARM9, VBlank (bit 0) has a slot too. */
void func_arm7_037fb748(OSIrqMask mask, OSIrqFunction function)
{
    int bit;

    for (bit = 0; bit < 25; mask >>= 1, bit++) {
        IrqSlotEntry *slot;

        if ((mask & 1) == 0) {
            continue;
        }
        slot = NULL;
        if (bit >= 8 && bit <= 11) {
            slot = &data_arm7_0380788c[bit - 8];
        } else if (bit >= 3 && bit <= 6) {
            slot = &data_arm7_0380788c[bit + 1];
        } else if (bit == 0) {
            slot = &data_arm7_038078ec;
        } else {
            data_arm7_03807750[bit] = function;
        }
        if (slot != NULL) {
            /* slot handlers take an argument, which is NULL here */
            slot->handler = (void (*)(void *))function;
            slot->arg = NULL;
            slot->keepEnabled = TRUE;
        }
    }
}
