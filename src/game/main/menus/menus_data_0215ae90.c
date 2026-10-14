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

TaskScriptCmd data_0215ae90[] = {
#include "data/main/data_0215ae90.inc.c"
};
MenuButtonTemplate data_0215aea8 = {
#include "data/main/data_0215aea8.inc.c"
};
TaskScriptCmd data_0215aed0[] = {
#include "data/main/data_0215aed0.inc.c"
};
TaskScriptCmd data_0215af10[] = {
#include "data/main/data_0215af10.inc.c"
};
