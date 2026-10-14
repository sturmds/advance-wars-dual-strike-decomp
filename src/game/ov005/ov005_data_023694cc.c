#include "types.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void Ov005_SetUpMessageScreen(void);
void Ov005_UpdateMessageScreen(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_ov005_0235788c(void);
void func_ov005_02358928(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

TaskScriptCmd data_ov005_023694cc[5] = {
#include "data/ov005/data_ov005_023694cc.inc.c"
};
/* declared `u8 gOv005MessageMainLayers[]` in ov005_set_up_message_screen.c: relocation at gOv005MessageMainLayers+0xc is not on a 4-byte field */
u32 gOv005MessageMainLayers[20] = {
#include "data/ov005/gOv005MessageMainLayers.inc.c"
};
/* declared `u8 gOv005MessageSubLayers[]` in ov005_set_up_message_screen.c: relocation at gOv005MessageSubLayers+0xc is not on a 4-byte field */
u32 gOv005MessageSubLayers[20] = {
#include "data/ov005/gOv005MessageSubLayers.inc.c"
};
