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
extern EventScriptCommand data_02167940[];
extern EventScriptCommand data_02167a40[];
extern EventScriptCommand data_02167b50[];
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

u8 data_021674c4[] = {
#include "data/main/data_021674c4.inc.c"
};
TaskScriptCmd data_021674c8[] = {
#include "data/main/data_021674c8.inc.c"
};
TaskScriptCmd gBattleRecordExitScript[] = {
#include "data/main/gBattleRecordExitScript.inc.c"
};
TaskScriptCmd data_021674e0[] = {
#include "data/main/data_021674e0.inc.c"
};
TaskScriptCmd data_02167520[] = {
#include "data/main/data_02167520.inc.c"
};
EventScriptCommand data_02167550[] = {
#include "data/main/data_02167550.inc.c"
};
EventScriptCommand data_02167590[] = {
#include "data/main/data_02167590.inc.c"
};
TaskScriptCmd gBattleRecordTaskScript[] = {
#include "data/main/gBattleRecordTaskScript.inc.c"
};
EventScriptCommand data_02167640[] = {
#include "data/main/data_02167640.inc.c"
};
TaskScriptCmd data_021676a0[] = {
#include "data/main/data_021676a0.inc.c"
};
EventScriptCommand data_02167700[] = {
#include "data/main/data_02167700.inc.c"
};
EventScriptCommand data_02167770[] = {
#include "data/main/data_02167770.inc.c"
};
EventScriptCommand data_021677f0[] = {
#include "data/main/data_021677f0.inc.c"
};
EventScriptCommand data_02167880[] = {
#include "data/main/data_02167880.inc.c"
};
EventScriptCommand data_02167940[] = {
#include "data/main/data_02167940.inc.c"
};
EventScriptCommand data_02167a40[] = {
#include "data/main/data_02167a40.inc.c"
};
EventScriptCommand data_02167b50[] = {
#include "data/main/data_02167b50.inc.c"
};
