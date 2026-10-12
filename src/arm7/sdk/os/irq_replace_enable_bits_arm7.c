#include "sdk/os_arm7.h"

/* ARM7 copy of OS_SetIrqMask: replaces IE with IME off. Returns the old IE. */
OSIrqMask Irq_ReplaceEnableBits_Arm7(OSIrqMask mask)
{
    u16 savedIme = *REG_IME;
    OSIrqMask old;

    *REG_IME = 0;
    old = *REG_IE;
    *REG_IE = mask;
    *REG_IME; /* needed to match: the original reads IME once before restoring it */
    *REG_IME = savedIme;
    return old;
}
