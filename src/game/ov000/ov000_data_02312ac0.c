#include "types.h"

extern u32 data_ov000_02312c04[];
extern u32 data_ov000_02312b94[];
extern u32 data_ov000_02312cfc[];
extern u32 data_ov000_02312b68[];
extern u32 data_ov000_02312b18[];
extern u32 data_ov000_02312b40[];

u32 data_ov000_02312ac0[] = {
#include "data/ov000/data_ov000_02312ac0.inc.c"
};
u32 data_ov000_02312adc[] = {
#include "data/ov000/data_ov000_02312adc.inc.c"
};
/* declared `u8 data_ov000_02312af8[]` in ov000_data_022f6bd8.c: relocation at data_ov000_02312af8+0x0 is not on a 4-byte field */
void* data_ov000_02312af8[] = {&data_ov000_02312c04, &data_ov000_02312b94,
                               &data_ov000_02312cfc, &data_ov000_02312ac0,
                               &data_ov000_02312b68, &data_ov000_02312b18,
                               &data_ov000_02312adc, &data_ov000_02312b40};
u32 data_ov000_02312b18[] = {
#include "data/ov000/data_ov000_02312b18.inc.c"
};
u32 data_ov000_02312b40[] = {
#include "data/ov000/data_ov000_02312b40.inc.c"
};
u32 data_ov000_02312b68[] = {
#include "data/ov000/data_ov000_02312b68.inc.c"
};
u32 data_ov000_02312b94[] = {
#include "data/ov000/data_ov000_02312b94.inc.c"
};
u32 data_ov000_02312c04[] = {
#include "data/ov000/data_ov000_02312c04.inc.c"
};
u32 data_ov000_02312cfc[] = {
#include "data/ov000/data_ov000_02312cfc.inc.c"
};
