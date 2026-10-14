#include "types.h"
#include "game/task.h"

void DayBanner_Cancel(void);
void DayBanner_CheckSkip(void);
void DayBanner_CloseUpdate(void);
void DayBanner_HoldUpdate(void);
void DayBanner_Init(void);
void DayBanner_LoadGraphics(void);
void DayBanner_OpenUpdate(void);
void DayBanner_RestoreScreen(void);
void DayBanner_SetUpBlend(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void func_020526e0(void);

/* "/ohashi/res_titlecall" */
char data_0215bf20[22] = {
#include "data/main/data_0215bf20.inc.c"
};
char data_0215bf38[27] = {
#include "data/main/data_0215bf38.inc.c"
};
TaskScriptCmd gDayBannerScript[6] = {
#include "data/main/gDayBannerScript.inc.c"
};
TaskScriptCmd data_0215bf84[8] = {
#include "data/main/data_0215bf84.inc.c"
};
TaskScriptCmd data_0215bfc4[8] = {
#include "data/main/data_0215bfc4.inc.c"
};
