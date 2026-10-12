#include "types.h"

extern u32 data_022a8048; /* bit n set: alarm n is in use */

/* Marks an alarm (0 to 7) as free again. */
void SoundAlarm_Free(int alarm) { data_022a8048 &= ~(1 << alarm); }
