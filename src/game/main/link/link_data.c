#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void LinkPhase_Draw(void);
void LinkPhase_InitScreen(void);
void LinkPhase_Update(void);
void LinkScreen_OnPlayerButtonConfirm(void);
void LinkScreen_OnReadyConfirm(void);
void LinkScreen_SavePanelOrder(void);
void Screen1_DestroyTextTasks(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_0215f8ec[];
void func_0207287c(void);
void func_02072f84(void);
void func_020737a8(void);
void func_02073868(void);

char data_0215f810[8] = {
#include "data/main/data_0215f810.inc.c"
};
u16 gLinkPhaseArmyGfxOffsets[6] = {
#include "data/main/gLinkPhaseArmyGfxOffsets.inc.c"
};
u32 data_0215f824[4] = {
#include "data/main/data_0215f824.inc.c"
};
MenuButtonTemplate data_0215f85c = {
#include "data/main/data_0215f85c.inc.c"
};
MenuButtonTemplate data_0215f884 = {
#include "data/main/data_0215f884.inc.c"
};
MenuButtonTemplate data_0215f834 = {
#include "data/main/data_0215f834.inc.c"
};
TaskScriptCmd data_0215f8ac[8] = {
#include "data/main/data_0215f8ac.inc.c"
};
TaskScriptCmd data_0215f8ec[11] = {
#include "data/main/data_0215f8ec.inc.c"
};
