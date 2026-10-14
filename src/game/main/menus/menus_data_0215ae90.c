#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void Menu_DismissOnPress(void);
void PageStripTask_Create(void);
void PageStripTask_Draw(void);
void PaletteCycle_Stop(void);
void Screen1_DestroyTextTasks(void);
void SurvivalProgress_Init(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_0215af10[];
void func_02048270(void);
void func_0204870c(void);

/* declared `u8 data_0215ae90[]` in page_strip_task_create.c: relocation at data_0215ae90+0xc is not on a 4-byte field */
u32 data_0215ae90[6] = {
#include "data/main/data_0215ae90.inc.c"
};
/* declared `u8 data_0215aea8[]` in func_02048250.c: relocation at data_0215aea8+0x18 is not on a 4-byte field */
u32 data_0215aea8[10] = {
#include "data/main/data_0215aea8.inc.c"
};
TaskScriptCmd data_0215aed0[8] = {
#include "data/main/data_0215aed0.inc.c"
};
TaskScriptCmd data_0215af10[9] = {
#include "data/main/data_0215af10.inc.c"
};
