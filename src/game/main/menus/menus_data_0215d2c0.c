#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void PlayerInfo_ClearTextLayer(void);
void PlayerInfo_DrawPage(void);
void PlayerInfo_DrawSprites(void);
void PlayerInfo_HandleKeys(void);
void PlayerInfo_Init(void);
void PlayerInfo_NextPage(void);
void PlayerInfo_PrevPage(void);
void PlayerInfo_SpriteTaskInit(void);
void PlayerInfo_ToggleLastPageView(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_0215d430[];
void func_0206d378(void);
void func_0206f7ec(void);
void func_0206f7f0(void);
void PlayerInfo_SlideCoPortrait(void);
void func_0206cf98(void);
void PlayerInfo_OnCoSelected(void);

u32 data_0215d2c0[] = {
#include "data/main/data_0215d2c0.inc.c"
};
u32 data_0215d2d0[] = {
#include "data/main/data_0215d2d0.inc.c"
};
u8 gUnitRowTypes[] = {
#include "data/main/gUnitRowTypes.inc.c"
};
MenuButtonTemplate data_0215d380 = {
#include "data/main/data_0215d380.inc.c"
};
MenuButtonTemplate data_0215d358 = {
#include "data/main/data_0215d358.inc.c"
};
MenuButtonTemplate data_0215d308 = {
#include "data/main/data_0215d308.inc.c"
};
MenuButtonTemplate data_0215d330 = {
#include "data/main/data_0215d330.inc.c"
};
MenuButtonTemplate data_0215d3a8 = {
#include "data/main/data_0215d3a8.inc.c"
};
TaskScriptCmd gPlayerInfoRedrawScript[] = {
#include "data/main/gPlayerInfoRedrawScript.inc.c"
};
TaskScriptCmd data_0215d400[] = {
#include "data/main/data_0215d400.inc.c"
};
TaskScriptCmd data_0215d430[] = {
#include "data/main/data_0215d430.inc.c"
};
