#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/event_script.h"
#include "game/menu_button.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void CampaignMap_CalloutShrink(void);
void CampaignMap_Cancel(void);
void CampaignMap_Confirm(void);
void CampaignMap_DelayedTaskAndWait(void);
void CampaignMap_DrawPanelText(void);
void CampaignMap_EaseScroll(void);
void CampaignMap_FreeMarkerSprites(void);
void CampaignMap_HandleKeys(void);
void CampaignMap_Init(void);
void CampaignMap_InitCrossingSprites(void);
void CampaignMap_StartMissionScript(void);
void CampaignMap_StartMissionScroll(void);
void CampaignMap_StartModeScript(void);
void CampaignMap_StartMusicForMode(void);
void CampaignMap_WaitThenAddButtons(void);
void Save_LoadProfiles(void);
void Save_LoadProgress(void);
void Screen1_DestroyTextTasks(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_021674c8[];
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_020199a4(void);
void func_02021b34(void);
void func_0204e6bc(void);
void func_0204e768(void);
void func_0204ebfc(void);
void func_0204ec0c(void);
void func_0204ec1c(void);
void func_0204ec28(void);
void func_0204ec64(void);
void func_0204ef30(void);
void func_0204f0c0(void);
void func_0204f3b0(void);
void func_0204f7c4(void);
void func_0204f94c(void);
void func_0204f95c(void);
void func_020b983c(void);
void func_020b9864(void);
void func_020b9890(void);
void func_02103714(void);
void CampaignMap_AfterEventScript(void);
void CampaignMap_CalloutGrow(void);
void CampaignMap_CenterOnMission(void);
void CampaignMap_CenterOnPlayedMission(void);
void CampaignMap_DrawCoPanel(void);
void CampaignMap_DrawDoneMarkers(void);
void CampaignMap_OffsetLayer1(void);
void CampaignMap_SlideInNumber(void);
void CampaignMap_StartMusic(void);
void CampaignMap_StartTaskAndWait(void);
void CampaignMap_UpdateCrossingSprites(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];
void func_0204e808(void);
void func_0204f200(void);
void CampaignMap_CalloutHold120(void);
void CampaignMap_CalloutHold60(void);
void CampaignMap_DrawMarkers(void);
void CampaignMap_DrawNumberAtMission(void);
void func_0204f160(void);

TaskScriptCmd data_0215b350[] = {
#include "data/main/data_0215b350.inc.c"
};
u32 data_0215b398[] = {
#include "data/main/data_0215b398.inc.c"
};
TaskScriptCmd data_0215b368[] = {
#include "data/main/data_0215b368.inc.c"
};
TaskScriptCmd data_0215b338[] = {
#include "data/main/data_0215b338.inc.c"
};
TaskScriptCmd data_0215b380[] = {
#include "data/main/data_0215b380.inc.c"
};
TaskScriptCmd data_0215b3b0[] = {
#include "data/main/data_0215b3b0.inc.c"
};
MenuButtonTemplate data_0215b448 = {
#include "data/main/data_0215b448.inc.c"
};
TaskScriptCmd data_0215b420[] = {
#include "data/main/data_0215b420.inc.c"
};
MenuButtonTemplate data_0215b3d0 = {
#include "data/main/data_0215b3d0.inc.c"
};
MenuButtonTemplate data_0215b3f8 = {
#include "data/main/data_0215b3f8.inc.c"
};
TaskScriptCmd data_0215b470[] = {
#include "data/main/data_0215b470.inc.c"
};
TaskScriptCmd data_0215b4a0[] = {
#include "data/main/data_0215b4a0.inc.c"
};
TaskScriptCmd data_0215b4d0[] = {
#include "data/main/data_0215b4d0.inc.c"
};
EventScriptCommand data_0215b508[] = {
#include "data/main/data_0215b508.inc.c"
};
GfxLayer data_0215b598[] = {
#include "data/main/data_0215b598.inc.c"
};
GfxLayer data_0215b548[] = {
#include "data/main/data_0215b548.inc.c"
};
TaskScriptCmd data_0215b5e8[] = {
#include "data/main/data_0215b5e8.inc.c"
};
TaskScriptCmd data_0215b648[] = {
#include "data/main/data_0215b648.inc.c"
};
EventScriptCommand data_0215b6b0[] = {
#include "data/main/data_0215b6b0.inc.c"
};
TaskScriptCmd data_0215b720[] = {
#include "data/main/data_0215b720.inc.c"
};
TaskScriptCmd data_0215b7b0[] = {
#include "data/main/data_0215b7b0.inc.c"
};
TaskScriptCmd data_0215b870[] = {
#include "data/main/data_0215b870.inc.c"
};
