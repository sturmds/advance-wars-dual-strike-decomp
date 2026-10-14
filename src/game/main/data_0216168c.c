#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void CombatResult_Close(void);
void CombatResult_Draw(void);
void CombatResult_FadeIn(void);
void CombatResult_Init(void);
void CombatResult_OnKeys(void);
void CombatSetup_SaveToBuffer(void);
void EventSequenceTask_StartWithSongFade(void);
void func_02038264(void);
void func_0208af80(void);
void func_0208b1dc(void);
void func_0208b97c(void);

TaskScriptCmd data_0216168c[] = {
#include "data/main/data_0216168c.inc.c"
};
MenuButtonTemplate data_0216169c = {
#include "data/main/data_0216169c.inc.c"
};
TaskScriptCmd data_021616c4[] = {
#include "data/main/data_021616c4.inc.c"
};
