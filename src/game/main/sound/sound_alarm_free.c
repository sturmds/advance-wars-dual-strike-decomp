#include "types.h"
#include "game/sound.h"

/* Marks an alarm (0 to 7) as free again. */
void SoundAlarm_Free(int alarm) { data_022a8048 &= ~(1 << alarm); }
