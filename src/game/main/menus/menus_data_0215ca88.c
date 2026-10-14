#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void BattleSetup_ApplyToGame(void);
void BattleSetup_LoadFromGame(void);
void HelpBand_Stop(void);
void Screen1_DestroyTextTasks(void);
void VsSetupSlotTask_GrowIn(void);
void VsSetupSlotTask_SquashOut(void);
void VsSetup_ChangeSettingDown(void);
void VsSetup_ChangeSettingUp(void);
void VsSetup_ClearReadyFlags(void);
void VsSetup_Confirm(void);
void VsSetup_DestroyPlayerTasks(void);
void VsSetup_DrawButtonSprites(void);
void VsSetup_DrawSettings(void);
void VsSetup_DrawSettingsTeamSprites(void);
void VsSetup_DrawSlotSprites(void);
void VsSetup_DrawTeamScreenTeamSprites(void);
void VsSetup_ForceRankTaskStart(void);
void VsSetup_HandleKeys(void);
void VsSetup_Init(void);
void VsSetup_NextForceRankSet(void);
void VsSetup_NextTeamForButton(void);
void VsSetup_OnSettingDownArrow(void);
void VsSetup_OnSettingRowButton(void);
void VsSetup_OnSettingUpArrow(void);
void VsSetup_OpenForceRank(void);
void VsSetup_PrevForceRankSet(void);
void VsSetup_PrevTeamForButton(void);
void VsSetup_ReopenSettingsMenu(void);
void VsSetup_RequestConfirmTeams(void);
void VsSetup_RequestTeamScreenBack(void);
void VsSetup_ReturnFromForceRank(void);
void VsSetup_RunPendingCancel(void);
void VsSetup_ShowMainScreen(void);
void VsSetup_ShowSettingsMenu(void);
void VsSetup_ShowTeamScreen(void);
void VsSetup_TeamSlotOnKeys(void);
void VsSetup_UpdateTeamScreenLink(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_0215cfe8[];
extern TaskScriptCmd data_0215d038[];
extern TaskScriptCmd data_0215d090[];
extern TaskScriptCmd data_0215d0f0[];
void func_020648b8(void);
void func_020648d0(void);
void func_02064904(void);
void func_02064910(void);
void func_02065464(void);
void func_020654a0(void);
void func_02066244(void);
void func_02069460(void);
void func_02069610(void);
void func_0206a324(void);
void func_0206b974(void);
void func_0206bb5c(void);
void func_0206bb68(void);
void func_0206bc44(void);
void func_020b983c(void);
void VsSetup_OnCancel(void);
void VsSetup_UpdateLink(void);
void VsSetup_UpdateSettingsMenu(void);
void func_02066d70(void);
void func_020678e0(void);
void VsSetup_OnButtonSelected(void);
void VsSetup_OnSlotButtonSelected(void);

char data_0215ca88[] = {
#include "data/main/data_0215ca88.inc.c"
};
/* declared `u8 gVsSetupSettingRowY[SETTING_ROWS]` in vs_setup_add_setting_buttons.c: declared in src/game/main/menus/vs_setup_add_setting_buttons.c, with a type no header defines */
u8 gVsSetupSettingRowY[] = {
#include "data/main/gVsSetupSettingRowY.inc.c"
};
/* declared `u8 gVsSetupSettingRowX[SETTING_ROWS]` in vs_setup_add_setting_buttons.c: declared in src/game/main/menus/vs_setup_add_setting_buttons.c, with a type no header defines */
u8 gVsSetupSettingRowX[] = {
#include "data/main/gVsSetupSettingRowX.inc.c"
};
TaskScriptCmd data_0215cad8[] = {
#include "data/main/data_0215cad8.inc.c"
};
TaskScriptCmd data_0215cac8[] = {
#include "data/main/data_0215cac8.inc.c"
};
TaskScriptCmd data_0215cab8[] = {
#include "data/main/data_0215cab8.inc.c"
};
TaskScriptCmd data_0215caa8[] = {
#include "data/main/data_0215caa8.inc.c"
};
TaskScriptCmd data_0215cae8[] = {
#include "data/main/data_0215cae8.inc.c"
};
MenuButtonTemplate data_0215cc38 = {
#include "data/main/data_0215cc38.inc.c"
};
MenuButtonTemplate data_0215cdc8 = {
#include "data/main/data_0215cdc8.inc.c"
};
MenuButtonTemplate data_0215cd28 = {
#include "data/main/data_0215cd28.inc.c"
};
MenuButtonTemplate data_0215cd50 = {
#include "data/main/data_0215cd50.inc.c"
};
MenuButtonTemplate data_0215ce40 = {
#include "data/main/data_0215ce40.inc.c"
};
MenuButtonTemplate data_0215ce18 = {
#include "data/main/data_0215ce18.inc.c"
};
MenuButtonTemplate data_0215cdf0 = {
#include "data/main/data_0215cdf0.inc.c"
};
MenuButtonTemplate data_0215cb98 = {
#include "data/main/data_0215cb98.inc.c"
};
MenuButtonTemplate data_0215cda0 = {
#include "data/main/data_0215cda0.inc.c"
};
MenuButtonTemplate data_0215cb70 = {
#include "data/main/data_0215cb70.inc.c"
};
MenuButtonTemplate data_0215cb20 = {
#include "data/main/data_0215cb20.inc.c"
};
MenuButtonTemplate data_0215cb48 = {
#include "data/main/data_0215cb48.inc.c"
};
MenuButtonTemplate data_0215cd00 = {
#include "data/main/data_0215cd00.inc.c"
};
MenuButtonTemplate data_0215cbe8 = {
#include "data/main/data_0215cbe8.inc.c"
};
TaskScriptCmd data_0215ccb0[] = {
#include "data/main/data_0215ccb0.inc.c"
};
MenuButtonTemplate data_0215cc10 = {
#include "data/main/data_0215cc10.inc.c"
};
MenuButtonTemplate data_0215cc60 = {
#include "data/main/data_0215cc60.inc.c"
};
MenuButtonTemplate data_0215cc88 = {
#include "data/main/data_0215cc88.inc.c"
};
MenuButtonTemplate data_0215ccd8 = {
#include "data/main/data_0215ccd8.inc.c"
};
MenuButtonTemplate data_0215cd78 = {
#include "data/main/data_0215cd78.inc.c"
};
MenuButtonTemplate data_0215cbc0 = {
#include "data/main/data_0215cbc0.inc.c"
};
MenuButtonTemplate data_0215caf8 = {
#include "data/main/data_0215caf8.inc.c"
};
TaskScriptCmd data_0215ce68[] = {
#include "data/main/data_0215ce68.inc.c"
};
TaskScriptCmd data_0215ce98[] = {
#include "data/main/data_0215ce98.inc.c"
};
TaskScriptCmd gVsSetupSlot0TaskScript[] = {
#include "data/main/gVsSetupSlot0TaskScript.inc.c"
};
TaskScriptCmd gVsSetupSlot1TaskScript[] = {
#include "data/main/gVsSetupSlot1TaskScript.inc.c"
};
TaskScriptCmd gVsSetupSlot2TaskScript[] = {
#include "data/main/gVsSetupSlot2TaskScript.inc.c"
};
TaskScriptCmd gVsSetupSlot3TaskScript[] = {
#include "data/main/gVsSetupSlot3TaskScript.inc.c"
};
TaskScriptCmd data_0215cfa8[] = {
#include "data/main/data_0215cfa8.inc.c"
};
TaskScriptCmd data_0215cfe8[] = {
#include "data/main/data_0215cfe8.inc.c"
};
TaskScriptCmd data_0215d038[] = {
#include "data/main/data_0215d038.inc.c"
};
TaskScriptCmd data_0215d0f0[] = {
#include "data/main/data_0215d0f0.inc.c"
};
TaskScriptCmd data_0215d090[] = {
#include "data/main/data_0215d090.inc.c"
};
TaskScriptCmd data_0215d150[] = {
#include "data/main/data_0215d150.inc.c"
};
TaskScriptCmd data_0215d1d8[] = {
#include "data/main/data_0215d1d8.inc.c"
};
