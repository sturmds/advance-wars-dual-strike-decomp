#include "types.h"
#include "text_data.h"

/* The values come from the ROM at build time (config/AWRE/data_inc.txt). */
#pragma section TEXT_DATA begin

/* CRC-16 (polynomial 0xa001) of each nibble, for CalcCrc16. */
u16 gCrc16NibbleTable[16] = {
#include "data/arm7/gCrc16NibbleTable.inc.c"
};

#pragma section TEXT_DATA end
