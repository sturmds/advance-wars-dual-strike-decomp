#include "types.h"

static u16 gOsiTimerReserved;

/* Marks hardware timer number bit as in use by the library. */
void OSi_SetTimerReserved(int bit) { gOsiTimerReserved |= 1 << bit; }
