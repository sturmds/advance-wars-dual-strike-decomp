#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void ForceRankScreen_Draw(void);
void ForceRankScreen_HandleKeys(void);
void ForceRankScreen_Init(void);
void ForceRankScreen_SelectSlot(void);
void Screen1_DestroyTextTasks(void);
void Wallpaper_DestroyTasks(void);
void func_0203ff2c(void);
void func_0203ff38(void);
void func_0203ff44(void);
void func_02040c34(void);
extern TaskScriptCmd gForceRankScreenScript[];
void ForceRankScreen_ToggleSkill(void);
void ForceRankScreen_OnSkillCancel(void);
void ForceRankScreen_OnSkillSelected(void);

u32 data_0215a794[] = {
#include "data/main/data_0215a794.inc.c"
};
MenuButtonTemplate gForceRankSlotButtonTemplate = {
#include "data/main/gForceRankSlotButtonTemplate.inc.c"
};
MenuButtonTemplate data_0215a81c = {
#include "data/main/data_0215a81c.inc.c"
};
MenuButtonTemplate data_0215a7f4 = {
#include "data/main/data_0215a7f4.inc.c"
};
MenuButtonTemplate data_0215a7cc = {
#include "data/main/data_0215a7cc.inc.c"
};
MenuButtonTemplate gForceRankSkillButtonTemplate = {
#include "data/main/gForceRankSkillButtonTemplate.inc.c"
};
TaskScriptCmd data_0215a86c[] = {
#include "data/main/data_0215a86c.inc.c"
};
TaskScriptCmd gForceRankScreenScript[] = {
#include "data/main/gForceRankScreenScript.inc.c"
};
