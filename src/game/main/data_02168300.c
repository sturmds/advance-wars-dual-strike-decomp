#include "types.h"
#include "game/task.h"
#include "game/script_slot.h"
#include "game/unit_target_entry.h"

void Battle_StartScreen(void);
void Combat_ApplyResult(void);
void Combat_WriteBackAttacker(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void ObjectAttackTask_Finish(void);
void ObjectAttack_ShowHit(void);
extern TaskScriptCmd data_02168f48[];
void func_02001540(void);
void func_0201d7a8(void);
void func_0201d7bc(void);
void func_020c06d4(void);
void func_020d3310(void);
void func_020dc55c(void);
void func_020dc698(void);
void func_020dc704(void);
void func_020dd138(void);
void func_020fb9ec(void);
extern ScriptCommand gMapScrollScript[];
void AttackTask_SetUpScreen(void);
void UnitAttackTask_Finish(void);
void func_02001564(void);
void func_020c06a4(void);
void func_020fb868(void);
void AttackTask_StartScene(void);
void func_020dc710(void);

/* "0a4" */
char data_02168308[] = {
#include "data/main/data_02168308.inc.c"
};
/* "0a9" */
char data_02168304[] = {
#include "data/main/data_02168304.inc.c"
};
/* "0a8" */
char data_02168300[] = {
#include "data/main/data_02168300.inc.c"
};
/* the buffer that the unit target list and the AI reach tiles share */
void* data_02168314 = {
#include "data/main/data_02168314.inc.c"
};
char data_02168310[] = {
#include "data/main/data_02168310.inc.c"
};
char data_0216830c[] = {
#include "data/main/data_0216830c.inc.c"
};
TaskScriptCmd data_02168318[] = {
#include "data/main/data_02168318.inc.c"
};
TaskScriptCmd data_02168328[] = {
#include "data/main/data_02168328.inc.c"
};
TaskScriptCmd data_02168380[] = {
#include "data/main/data_02168380.inc.c"
};
TaskScriptCmd data_02168400[] = {
#include "data/main/data_02168400.inc.c"
};
