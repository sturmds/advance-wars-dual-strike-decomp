#include "types.h"
#include "game/script_slot.h"
#include "game/event_script.h"
#include "game/task.h"

void CoSwapTask_ShowLine(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void RepairTask_ServiceUnit(void);
extern ScriptCommand data_021666e8[];
void func_02055c94(void);
void func_0205c014(void);
void func_020c4018(void);
void func_020dd370(void);
void func_020dd5dc(void);
void func_020dd5f8(void);
void Game_EnterState18(void);

/* dx, dy per direction */
s16 data_021684e8[] = {
#include "data/main/data_021684e8.inc.c"
};
/* dx, dy per direction */
s16 data_021684d8[] = {
#include "data/main/data_021684d8.inc.c"
};
TaskScriptCmd data_02168538[] = {
#include "data/main/data_02168538.inc.c"
};
TaskScriptCmd data_021684f8[] = {
#include "data/main/data_021684f8.inc.c"
};
TaskScriptCmd data_02168578[] = {
#include "data/main/data_02168578.inc.c"
};
EventScriptCommand data_021685c8[] = {
#include "data/main/data_021685c8.inc.c"
};
