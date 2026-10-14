#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void CoSelectPictureTask_Init(void);
void CoSelectScreen_Create(void);
void CoSelectScreen_DestroyTasks(void);
void CoSelectScreen_Draw(void);
void CoSelectScreen_DrawColumnInfo(void);
void CoSelectScreen_HandleCoListKeys(void);
void CoSelectScreen_HandleKeys(void);
void CoSelectScreen_Init(void);
void CoSelectScreen_OpenForceRank(void);
void CoSelectScreen_RestoreButtons(void);
void CoSelectScreen_StartSong(void);
void CoSelect_ConfirmPicks(void);
void Task_ResumeWhenScriptIdle(void);
extern TaskScriptCmd data_0215ab00[];
void func_02041864(void);
void func_020418cc(void);
void func_02041a94(void);
void func_02041aa0(void);
void func_02041aac(void);
void func_02041ad4(void);
void func_02042d7c(void);
void func_02042da4(void);
void func_02042e9c(void);
void func_020441e8(void);
void CoSelectScreen_ConfirmCoList(void);
void CoSelectScreen_OpenCoList(void);
void CoSelectScreen_SlidePictures(void);
void CoSelectScreen_CancelCoList(void);
void func_020428f4(void);
void CoSelectScreen_OnGridButtonSelect(void);
void CoSelectScreen_SelectCoListSlot(void);

TaskScriptCmd data_0215a900[] = {
#include "data/main/data_0215a900.inc.c"
};
TaskScriptCmd data_0215a928[] = {
#include "data/main/data_0215a928.inc.c"
};
TaskScriptCmd data_0215a910[] = {
#include "data/main/data_0215a910.inc.c"
};
TaskScriptCmd data_0215a940[] = {
#include "data/main/data_0215a940.inc.c"
};
TaskScriptCmd data_0215a960[] = {
#include "data/main/data_0215a960.inc.c"
};
TaskScriptCmd data_0215a980[] = {
#include "data/main/data_0215a980.inc.c"
};
MenuButtonTemplate gCoGridButtonTemplate = {
#include "data/main/gCoGridButtonTemplate.inc.c"
};
MenuButtonTemplate gCoListButtonTemplate = {
#include "data/main/gCoListButtonTemplate.inc.c"
};
MenuButtonTemplate data_0215a9a0 = {
#include "data/main/data_0215a9a0.inc.c"
};
MenuButtonTemplate data_0215a9c8 = {
#include "data/main/data_0215a9c8.inc.c"
};
MenuButtonTemplate data_0215a9f0 = {
#include "data/main/data_0215a9f0.inc.c"
};
MenuButtonTemplate data_0215aa68 = {
#include "data/main/data_0215aa68.inc.c"
};
TaskScriptCmd data_0215aa90[] = {
#include "data/main/data_0215aa90.inc.c"
};
u32 data_0215aac0[] = {
#include "data/main/data_0215aac0.inc.c"
};
TaskScriptCmd data_0215ab00[] = {
#include "data/main/data_0215ab00.inc.c"
};
