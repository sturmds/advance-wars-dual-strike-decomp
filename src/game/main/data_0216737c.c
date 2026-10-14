#include "types.h"
#include "game/task.h"

void LinkMapRecordSync_Start(void);
void LinkMode_AdvanceState(void);
void LinkMode_Finish(void);
void LinkTask_StartExchange(void);
void LinkTask_WaitForPeer(void);
void Link_AssignSlots(void);
void Link_CreateChildTask(void);
void Link_FillEmptySlotsWithAi(void);
void Link_SetSlotControllers(void);
void LoadingScreen_Init(void);
void Map_InitBattle(void);
void Screen1_DestroyTextTasks(void);
void VersusTask_ResetGame(void);
void VersusTask_SetDefaultRules(void);
void func_0202cbcc(void);
void func_0205cd80(void);
void func_02064820(void);
void func_02080684(void);
void func_020811b4(void);
void func_020811e4(void);
void func_020b983c(void);
void func_020d6a34(void);
void func_020d6a64(void);
void func_020d6a94(void);
void func_020e33e4(void);

TaskScriptCmd data_0216737c[] = {
#include "data/main/data_0216737c.inc.c"
};
TaskScriptCmd data_021673a4[] = {
#include "data/main/data_021673a4.inc.c"
};
TaskScriptCmd data_021673dc[] = {
#include "data/main/data_021673dc.inc.c"
};
