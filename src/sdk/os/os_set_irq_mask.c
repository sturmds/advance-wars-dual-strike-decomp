#include "sdk/os.h"

OSIrqMask OS_SetIrqMask(OSIrqMask mask)
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
