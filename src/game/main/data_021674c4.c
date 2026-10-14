#include "types.h"
#include "game/event_script.h"
#include "game/task.h"

void BattleRecordTask_ExitIfPlayerOut(void);
void BattleRecordTask_RunCallback(void);
void CallbackSlots_Clear(void);
void EventScript_IsRunning(void);
void EventSequenceTask_FadeOutSong(void);
void EventSequence_RunRecordStep(void);
void EventSequence_StartScript(void);
void Game_IsCampaign(void);
void MainLoop_Shutdown(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PaletteAnimTask_LoadFrame(void);
void PaletteAnimTask_Step(void);
void Save_EraseAll(void);
void Save_SaveCurrentGame(void);
void Sound_PlayPendingSong(void);
void Wallpaper_DestroyTasks(void);
extern u8 data_02158344[];
extern u32 data_02167940[];
extern u32 data_02167a40[];
extern u32 data_02167b50[];
void func_01ff95fc(void);
void func_01ff9610(void);
void func_02018e84(void);
void func_020c4018(void);
void func_020d7bf0(void);
void func_020d7c04(void);
void func_020d7e98(void);
void func_020d7ea4(void);
void func_020d7ed0(void);
void func_020d7ee0(void);
void func_020d80a8(void);
void func_020d80c4(void);
void func_020d8148(void);
void CampaignMap_RefreshLayers(void);
void EventChoice_AnswerIsNotLast(void);
void func_020199a4(void);
void func_020d7eb8(void);

u8 data_021674c4[4] = {
#include "data/main/data_021674c4.inc.c"
};
/* declared `u8 data_021674c8[]` in menus_data_0215b338.c: relocation at data_021674c8+0x4 is not on a 4-byte field */
u32 data_021674c8[6] = {
#include "data/main/data_021674c8.inc.c"
};
TaskScriptCmd gBattleRecordExitScript[4] = {
#include "data/main/gBattleRecordExitScript.inc.c"
};
TaskScriptCmd data_021674e0[4] = {
#include "data/main/data_021674e0.inc.c"
};
TaskScriptCmd data_02167520[6] = {
#include "data/main/data_02167520.inc.c"
};
u32 data_02167550[16] = {
#include "data/main/data_02167550.inc.c"
};
u32 data_02167590[20] = {
#include "data/main/data_02167590.inc.c"
};
TaskScriptCmd gBattleRecordTaskScript[12] = {
#include "data/main/gBattleRecordTaskScript.inc.c"
};
u32 data_02167640[24] = {
#include "data/main/data_02167640.inc.c"
};
TaskScriptCmd data_021676a0[12] = {
#include "data/main/data_021676a0.inc.c"
};
u32 data_02167700[28] = {
#include "data/main/data_02167700.inc.c"
};
u32 data_02167770[32] = {
#include "data/main/data_02167770.inc.c"
};
/* declared `u8 data_021677f0[]` in func_020d80c4.c: relocation at data_021677f0+0x34 is not on a 4-byte field */
u32 data_021677f0[36] = {
#include "data/main/data_021677f0.inc.c"
};
EventScriptCommand data_02167880[12] = {
#include "data/main/data_02167880.inc.c"
};
u32 data_02167940[64] = {
#include "data/main/data_02167940.inc.c"
};
u32 data_02167a40[68] = {
#include "data/main/data_02167a40.inc.c"
};
u32 data_02167b50[72] = {
#include "data/main/data_02167b50.inc.c"
};
