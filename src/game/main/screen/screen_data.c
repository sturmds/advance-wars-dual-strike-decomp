#include "types.h"
#include "game/screen_contexts.h"

extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

/* declared `u8 data_02157618[]` in screens_init_contexts.c: relocation at data_02157618+0xc is not on a 4-byte field */
u32 data_02157618[20] = {
#include "data/main/data_02157618.inc.c"
};
/* declared `u8 data_021575c8[]` in screens_init_contexts.c: relocation at data_021575c8+0xc is not on a 4-byte field */
u32 data_021575c8[20] = {
#include "data/main/data_021575c8.inc.c"
};
/* declared `u8 data_02157668[]` in screens_set_up2d_layers.c: relocation at data_02157668+0xc is not on a 4-byte field */
u32 data_02157668[20] = {
#include "data/main/data_02157668.inc.c"
};
