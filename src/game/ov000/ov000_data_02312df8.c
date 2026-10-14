#include "types.h"

extern u32 data_ov000_02312e04[];
extern u32 data_ov000_02312f14[];
extern u32 data_ov000_023130a0[];

/* declared `u8 data_ov000_02312df8[]` in ov000_data_022f6bd8.c: relocation at data_ov000_02312df8+0x0 is not on a 4-byte field */
void* data_ov000_02312df8[3] = {&data_ov000_02312e04, &data_ov000_02312f14,
                                &data_ov000_023130a0};
u32 data_ov000_02312e04[68] = {
#include "data/ov000/data_ov000_02312e04.inc.c"
};
u32 data_ov000_02312f14[99] = {
#include "data/ov000/data_ov000_02312f14.inc.c"
};
u32 data_ov000_023130a0[174] = {
#include "data/ov000/data_ov000_023130a0.inc.c"
};
