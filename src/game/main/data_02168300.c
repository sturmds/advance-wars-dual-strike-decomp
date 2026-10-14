#include "types.h"
#include "game/task.h"

void Battle_StartScreen(void);
void Combat_ApplyResult(void);
void Combat_WriteBackAttacker(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void ObjectAttackTask_Finish(void);
void ObjectAttack_ShowHit(void);
extern u8 data_02168f48[];
extern u8 data_022a6dd8[];
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
extern u8 gMapScrollScript[];
void AttackTask_SetUpScreen(void);
void UnitAttackTask_Finish(void);
void func_02001564(void);
void func_020c06a4(void);
void func_020fb868(void);
void AttackTask_StartScene(void);
void func_020dc710(void);

/* "0a4" */
char data_02168308[4] = {
#include "data/main/data_02168308.inc.c"
};
/* "0a9" */
char data_02168304[4] = {
#include "data/main/data_02168304.inc.c"
};
/* "0a8" */
char data_02168300[4] = {
#include "data/main/data_02168300.inc.c"
};
int data_02168314 = {
#include "data/main/data_02168314.inc.c"
};
char data_02168310[4] = {
#include "data/main/data_02168310.inc.c"
};
char data_0216830c[4] = {
#include "data/main/data_0216830c.inc.c"
};
TaskScriptCmd data_02168318[2] = {
#include "data/main/data_02168318.inc.c"
};
/* declared `u8 data_02168328[]` in func_020f38ac.c: relocation at data_02168328+0x4 is not on a 4-byte field */
u32 data_02168328[22] = {
#include "data/main/data_02168328.inc.c"
};
TaskScriptCmd data_02168380[16] = {
#include "data/main/data_02168380.inc.c"
};
/* declared `u8 data_02168400[]` in func_020f38ac.c: relocation at data_02168400+0x4 is not on a 4-byte field */
u32 data_02168400[54] = {
#include "data/main/data_02168400.inc.c"
};
