#include "types.h"
#include "game/zoom_sprite_task.h"

const u8 data_021388f0[64] = {
#include "data/main/data_021388f0.inc.c"
};
/* sin 0 to 90 degrees, 16384 = 1.0 */
const u16 gSinTable[92] = {
#include "data/main/gSinTable.inc.c"
};
const s16 gSineTable[320] = {
#include "data/main/gSineTable.inc.c"
};
