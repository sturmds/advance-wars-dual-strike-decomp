#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void PaletteCycle_Stop(void);
void Screen1_DestroyTextTasks(void);
void SurvivalModeMenu_Draw(void);
void SurvivalModeMenu_Init(void);
void SurvivalModeMenu_OnKeys(void);
void SurvivalSelect_CancelOnKey(void);
void SurvivalSelect_Create(void);
void SurvivalSelect_Draw(void);
void SurvivalSelect_Init(void);
void SurvivalSelect_MoveCursorOnKeys(void);
void SurvivalSelect_PickMap(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_0215ad6c[];
extern TaskScriptCmd data_0215adec[];
void func_020458a8(void);
void func_020459cc(void);
void func_02046170(void);
void func_02046dac(void);
void func_02046db8(void);
void func_02046e18(void);
void func_02046fa0(void);
void func_02046fc8(void);
void func_02047164(void);
void func_02047168(void);
void func_02045a9c(void);
void func_02046e64(void);
void func_02046e3c(void);

u32 data_0215ac14[4] = {
#include "data/main/data_0215ac14.inc.c"
};
u32 data_0215ac04[4] = {
#include "data/main/data_0215ac04.inc.c"
};
TaskScriptCmd data_0215acc4[5] = {
#include "data/main/data_0215acc4.inc.c"
};
/* declared `u8 data_0215ad14[]` in page_strip_add_buttons.c: relocation at data_0215ad14+0x14 is not on a 4-byte field */
u32 data_0215ad14[10] = {
#include "data/main/data_0215ad14.inc.c"
};
/* declared `u8 data_0215acec[]` in func_02046d68.c: relocation at data_0215acec+0x18 is not on a 4-byte field */
u32 data_0215acec[10] = {
#include "data/main/data_0215acec.inc.c"
};
/* declared `u8 data_0215ac74[]` in func_02046d68.c: relocation at data_0215ac74+0x18 is not on a 4-byte field */
u32 data_0215ac74[10] = {
#include "data/main/data_0215ac74.inc.c"
};
/* declared `u8 data_0215ac9c[]` in func_02046dc4.c: relocation at data_0215ac9c+0x14 is not on a 4-byte field */
u32 data_0215ac9c[10] = {
#include "data/main/data_0215ac9c.inc.c"
};
/* declared `u8 data_0215ac24[]` in page_strip_add_buttons.c: relocation at data_0215ac24+0x14 is not on a 4-byte field */
u32 data_0215ac24[10] = {
#include "data/main/data_0215ac24.inc.c"
};
/* declared `u8 data_0215ac4c[]` in func_020458c0.c: relocation at data_0215ac4c+0x18 is not on a 4-byte field */
u32 data_0215ac4c[10] = {
#include "data/main/data_0215ac4c.inc.c"
};
TaskScriptCmd data_0215ad3c[6] = {
#include "data/main/data_0215ad3c.inc.c"
};
TaskScriptCmd data_0215ad6c[8] = {
#include "data/main/data_0215ad6c.inc.c"
};
TaskScriptCmd data_0215adac[8] = {
#include "data/main/data_0215adac.inc.c"
};
TaskScriptCmd data_0215adec[14] = {
#include "data/main/data_0215adec.inc.c"
};
