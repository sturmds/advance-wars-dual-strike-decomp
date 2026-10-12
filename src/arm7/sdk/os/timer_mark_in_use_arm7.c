#include "types.h"

extern u16
    data_arm7_03807a78; /* bit n set: hardware timer n is in use by the library */

/* ARM7 copy of OSi_SetTimerReserved. Marks hardware timer number bit as in use by the library. */
void Timer_MarkInUse_Arm7(int bit) { data_arm7_03807a78 |= 1 << bit; }
