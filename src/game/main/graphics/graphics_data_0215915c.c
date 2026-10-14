#include "types.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void WallpaperSingleTask_Update(void);
void WallpaperTask_Init(void);
void WallpaperTask_InitScrolled(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void WallpaperTask_Update(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

u8 data_02159168[12] = {
#include "data/main/data_02159168.inc.c"
};
u8 data_0215915c[12] = {
#include "data/main/data_0215915c.inc.c"
};
char data_02159174[13] = {
#include "data/main/data_02159174.inc.c"
};
char data_02159194[13] = {
#include "data/main/data_02159194.inc.c"
};
char data_021591a4[13] = {
#include "data/main/data_021591a4.inc.c"
};
char data_02159184[13] = {
#include "data/main/data_02159184.inc.c"
};
char data_021591d4[14] = {
#include "data/main/data_021591d4.inc.c"
};
char data_021591e4[14] = {
#include "data/main/data_021591e4.inc.c"
};
char data_021591c4[14] = {
#include "data/main/data_021591c4.inc.c"
};
char data_021591f4[14] = {
#include "data/main/data_021591f4.inc.c"
};
char data_02159204[14] = {
#include "data/main/data_02159204.inc.c"
};
char data_02159214[14] = {
#include "data/main/data_02159214.inc.c"
};
char data_021591b4[14] = {
#include "data/main/data_021591b4.inc.c"
};
char data_02159224[15] = {
#include "data/main/data_02159224.inc.c"
};
char data_02159234[15] = {
#include "data/main/data_02159234.inc.c"
};
char data_02159244[15] = {
#include "data/main/data_02159244.inc.c"
};
char data_021592a4[15] = {
#include "data/main/data_021592a4.inc.c"
};
char data_02159254[15] = {
#include "data/main/data_02159254.inc.c"
};
char data_02159264[15] = {
#include "data/main/data_02159264.inc.c"
};
char data_02159274[15] = {
#include "data/main/data_02159274.inc.c"
};
char data_02159284[15] = {
#include "data/main/data_02159284.inc.c"
};
char data_02159294[15] = {
#include "data/main/data_02159294.inc.c"
};
char data_021592b4[16] = {
#include "data/main/data_021592b4.inc.c"
};
char data_021592c4[16] = {
#include "data/main/data_021592c4.inc.c"
};
char data_021592d4[16] = {
#include "data/main/data_021592d4.inc.c"
};
char data_021592e4[16] = {
#include "data/main/data_021592e4.inc.c"
};
char data_021592f4[16] = {
#include "data/main/data_021592f4.inc.c"
};
char data_02159318[17] = {
#include "data/main/data_02159318.inc.c"
};
char data_02159304[17] = {
#include "data/main/data_02159304.inc.c"
};
char data_0215932c[18] = {
#include "data/main/data_0215932c.inc.c"
};
char data_02159340[18] = {
#include "data/main/data_02159340.inc.c"
};
char data_02159354[18] = {
#include "data/main/data_02159354.inc.c"
};
char data_02159368[19] = {
#include "data/main/data_02159368.inc.c"
};
TaskScriptCmd data_0215937c[4] = {
#include "data/main/data_0215937c.inc.c"
};
/* declared `u8 gWallpaperTaskScript[]` in wallpaper_destroy_tasks.c: relocation at gWallpaperTaskScript+0xc is not on a 4-byte field */
u32 gWallpaperTaskScript[8] = {
#include "data/main/gWallpaperTaskScript.inc.c"
};
/* per wallpaper: offset of its tile map */
u16 data_021593fc[32] = {
#include "data/main/data_021593fc.inc.c"
};
/* per wallpaper: offset of its palette */
u16 data_021593bc[32] = {
#include "data/main/data_021593bc.inc.c"
};
/* declared `u8 data_0215943c[]` in gfx_init_both_screens.c: relocation at data_0215943c+0xc is not on a 4-byte field */
u32 data_0215943c[20] = {
#include "data/main/data_0215943c.inc.c"
};
/* declared `u8 data_021594dc[]` in func_02028978.c: relocation at data_021594dc+0xc is not on a 4-byte field */
u32 data_021594dc[20] = {
#include "data/main/data_021594dc.inc.c"
};
/* declared `u8 data_0215952c[]` in gfx_init_both_screens.c: relocation at data_0215952c+0xc is not on a 4-byte field */
u32 data_0215952c[20] = {
#include "data/main/data_0215952c.inc.c"
};
/* declared `u8 data_0215948c[]` in func_02028a30.c: relocation at data_0215948c+0xc is not on a 4-byte field */
u32 data_0215948c[20] = {
#include "data/main/data_0215948c.inc.c"
};
/* declared `u8 data_0215957c[]` in func_02028978.c: relocation at data_0215957c+0xc is not on a 4-byte field */
u32 data_0215957c[20] = {
#include "data/main/data_0215957c.inc.c"
};
/* "res_wall_default", "res_wall_cathy", ... */
const char* data_021595cc[31] = {
    (char*)&data_02159318, (char*)&data_02159254, (char*)&data_02159174,
    (char*)&data_021591a4, (char*)&data_021591b4, (char*)&data_021592d4,
    (char*)&data_02159294, (char*)&data_02159368, (char*)&data_02159234,
    (char*)&data_02159274, (char*)&data_02159194, (char*)&data_02159354,
    (char*)&data_02159204, (char*)&data_02159184, (char*)&data_021592b4,
    (char*)&data_02159224, (char*)&data_021592a4, (char*)&data_021591e4,
    (char*)&data_02159244, (char*)&data_0215932c, (char*)&data_02159214,
    (char*)&data_021592c4, (char*)&data_02159284, (char*)&data_02159340,
    (char*)&data_021592e4, (char*)&data_021592f4, (char*)&data_021591f4,
    (char*)&data_02159264, (char*)&data_021591d4, (char*)&data_021591c4,
    (char*)&data_02159304};
