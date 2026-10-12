#include "sdk/os.h"

/* ARM7 copy of OS_ResetRequestIrqMask: writing a 1 bit to IF clears that pending interrupt. Returns the old IF. */
OSIrqMask func_arm7_037fb620(OSIrqMask mask)
{
    u16 savedIme = *REG_IME;
    OSIrqMask old;

    *REG_IME = 0;
    old = *REG_IF;
    *REG_IF = mask;
    *REG_IME; /* needed to match: the original reads IME once before restoring it */
    *REG_IME = savedIme;
    return old;
}
