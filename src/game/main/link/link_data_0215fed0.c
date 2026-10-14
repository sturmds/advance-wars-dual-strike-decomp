#include "types.h"
#include "game/link_menu_state.h"
#include "game/menu_button.h"
#include "game/task.h"

void LinkBanner_Init(void);
void LinkMenu_EndScreen(void);
void LinkMenu_InitScreen(void);
void LinkMenu_OnKeys(void);
void LinkMenu_ResumeScript(void);
void LinkMenu_UpdatePlayerList(void);
void LinkMenu_WaitForChildAcks(void);
void LinkSubScreen_ClearLayer0(void);
void func_0207c380(void);
void func_0207c414(void);
void func_0207c6d8(void);
void func_0207c6e4(void);
void func_0207cc38(void);
void func_0207cea0(void);
void func_0207cee8(void);
void func_0207cef4(void);
void func_0207cf04(void);
void LinkBanner_GrowBox(void);
void LinkBanner_ShrinkBox(void);
void func_0207c6f0(void);
void LinkMenu_OnButtonSelect(void);

LinkMenuState* data_0215fed0 = &data_0229d88c;
TaskScriptCmd data_0215fed4[] = {
#include "data/main/data_0215fed4.inc.c"
};
TaskScriptCmd data_0215fee4[] = {
#include "data/main/data_0215fee4.inc.c"
};
MenuButtonTemplate data_0215ff4c = {
#include "data/main/data_0215ff4c.inc.c"
};
MenuButtonTemplate data_0215fefc = {
#include "data/main/data_0215fefc.inc.c"
};
MenuButtonTemplate data_0215ff24 = {
#include "data/main/data_0215ff24.inc.c"
};
MenuButtonTemplate data_0215ff74 = {
#include "data/main/data_0215ff74.inc.c"
};
TaskScriptCmd data_0215ff9c[] = {
#include "data/main/data_0215ff9c.inc.c"
};
TaskScriptCmd data_0215ffdc[] = {
#include "data/main/data_0215ffdc.inc.c"
};
TaskScriptCmd data_02160044[] = {
#include "data/main/data_02160044.inc.c"
};
