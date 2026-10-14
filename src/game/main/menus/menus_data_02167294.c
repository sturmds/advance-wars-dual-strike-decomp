#include "types.h"
#include "game/task.h"

void BattleEnd_AwardCoins(void);
void BattleEnd_UpdateRecords(void);
void BattleRecordTask_IsRunning(void);
void Battle_ShowResult(void);
void CampaignTask_SetClearFlags(void);
void GameMode_FinishMap(void);
void GameMode_StartRecordTask(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Save_LoadProgress(void);
void Survival_AwardCoExperience(void);
void Survival_ResumeRun(void);
void Survival_StartVariant(void);
void func_0203b300(void);
void func_02048d98(void);
void func_020b983c(void);
void func_020d331c(void);
void func_020eb2d4(void);

TaskScriptCmd data_02167294[4] = {
#include "data/main/data_02167294.inc.c"
};
TaskScriptCmd data_021672b4[12] = {
#include "data/main/data_021672b4.inc.c"
};
TaskScriptCmd data_02167314[13] = {
#include "data/main/data_02167314.inc.c"
};
