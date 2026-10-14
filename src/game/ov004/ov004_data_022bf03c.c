#include "types.h"
#include "game/screen_contexts.h"

extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

/* declared `u8 data_ov004_022bf03c[]` in ov004_task_init_blended_scene.c: relocation at data_ov004_022bf03c+0xc is not on a 4-byte field */
u32 data_ov004_022bf03c[] = {
#include "data/ov004/data_ov004_022bf03c.inc.c"
};
/* declared `u8 data_ov004_022bf08c[]` in ov004_task_init_blended_scene.c: relocation at data_ov004_022bf08c+0xc is not on a 4-byte field */
u32 data_ov004_022bf08c[] = {
#include "data/ov004/data_ov004_022bf08c.inc.c"
};
