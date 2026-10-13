#include "types.h"
#include "text_data.h"

/* The values come from the ROM at build time (config/AWRE/data_inc.txt). */
#pragma section TEXT_DATA begin

/* {IE mask, handler index} pairs, highest priority first, that
 * IrqNest_DispatchByPriority walks to pick the interrupt to serve. */
u32 gOsNestedIrqPriorities[22][2] = {
#include "data/arm7/gOsNestedIrqPriorities.inc.c"
};

#pragma section TEXT_DATA end
