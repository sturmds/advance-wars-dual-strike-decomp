#include "types.h"
#include "game/gfx_queue.h"

#pragma explicit_zero_data on
/* entries queued */
volatile int data_027c051c = {
#include "data/dtcm/data_027c051c.inc.c"
};
/* bytes queued */
volatile u32 data_027c0518 = {
#include "data/dtcm/data_027c0518.inc.c"
};
GfxQueueEntry data_027c0520[32] = {
#include "data/dtcm/data_027c0520.inc.c"
};
#pragma explicit_zero_data reset
