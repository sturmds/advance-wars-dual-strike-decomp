#include "types.h"

extern u32 data_ov000_0234e788[];
extern u32 data_ov000_0234e840[];
extern u32 data_ov000_0234e928[];
extern u32 data_ov000_0234ea1c[];

/* declared `u8 data_ov000_0234e778[]` in ov000_data_022f6bd8.c: relocation at data_ov000_0234e778+0x0 is not on a 4-byte field */
void* data_ov000_0234e778[4] = {&data_ov000_0234e788, &data_ov000_0234e840,
                                &data_ov000_0234e928, &data_ov000_0234ea1c};
u32 data_ov000_0234e788[46] = {
#include "data/ov000/data_ov000_0234e788.inc.c"
};
u32 data_ov000_0234e840[58] = {
#include "data/ov000/data_ov000_0234e840.inc.c"
};
u32 data_ov000_0234e928[61] = {
#include "data/ov000/data_ov000_0234e928.inc.c"
};
u32 data_ov000_0234ea1c[81] = {
#include "data/ov000/data_ov000_0234ea1c.inc.c"
};
