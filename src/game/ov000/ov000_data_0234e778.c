#include "types.h"

extern u32 data_ov000_0234e788[];
extern u32 data_ov000_0234e840[];
extern u32 data_ov000_0234e928[];
extern u32 data_ov000_0234ea1c[];

/* declared `u8 data_ov000_0234e778[]` in ov000_data_022f6bd8.c: relocation at data_ov000_0234e778+0x0 is not on a 4-byte field */
void* data_ov000_0234e778[] = {&data_ov000_0234e788, &data_ov000_0234e840,
                               &data_ov000_0234e928, &data_ov000_0234ea1c};
u32 data_ov000_0234e788[] = {
#include "data/ov000/data_ov000_0234e788.inc.c"
};
u32 data_ov000_0234e840[] = {
#include "data/ov000/data_ov000_0234e840.inc.c"
};
u32 data_ov000_0234e928[] = {
#include "data/ov000/data_ov000_0234e928.inc.c"
};
u32 data_ov000_0234ea1c[] = {
#include "data/ov000/data_ov000_0234ea1c.inc.c"
};
