#include "sdk/os.h"

/* Writing a 1 bit to IF clears that pending interrupt. Returns the old IF. */
OSIrqMask OS_ResetRequestIrqMask(OSIrqMask mask)
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
