#include "types.h"
#include "game/co_data.h"

/* Nation (1-5) of a CO, from its record. */
int Co_GetNation(int co) { return gCoTable[co].nation; }
