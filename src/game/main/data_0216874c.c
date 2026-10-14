#include "types.h"
#include "game/task.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PowerBonus_ApplyNextUnit(void);
void PowerHighlightTask_Update(void);
void Power_ResetScrollAndRefresh(void);
void func_020d5b0c(void);

char data_02168754[4] = {
#include "data/main/data_02168754.inc.c"
};
char data_02168750[4] = {
#include "data/main/data_02168750.inc.c"
};
char data_02168758[4] = {
#include "data/main/data_02168758.inc.c"
};
u8 data_0216874c[4] = {
#include "data/main/data_0216874c.inc.c"
};
char data_021687a4[4] = {
#include "data/main/data_021687a4.inc.c"
};
char data_021687a0[4] = {
#include "data/main/data_021687a0.inc.c"
};
char data_0216879c[4] = {
#include "data/main/data_0216879c.inc.c"
};
char data_02168798[4] = {
#include "data/main/data_02168798.inc.c"
};
char data_02168794[4] = {
#include "data/main/data_02168794.inc.c"
};
char data_02168790[4] = {
#include "data/main/data_02168790.inc.c"
};
char data_0216878c[4] = {
#include "data/main/data_0216878c.inc.c"
};
char data_02168788[4] = {
#include "data/main/data_02168788.inc.c"
};
char data_02168784[4] = {
#include "data/main/data_02168784.inc.c"
};
char data_02168780[4] = {
#include "data/main/data_02168780.inc.c"
};
char data_0216877c[4] = {
#include "data/main/data_0216877c.inc.c"
};
char data_02168778[4] = {
#include "data/main/data_02168778.inc.c"
};
char data_02168774[4] = {
#include "data/main/data_02168774.inc.c"
};
char data_02168770[4] = {
#include "data/main/data_02168770.inc.c"
};
char data_0216876c[4] = {
#include "data/main/data_0216876c.inc.c"
};
char data_02168768[4] = {
#include "data/main/data_02168768.inc.c"
};
char data_02168764[4] = {
#include "data/main/data_02168764.inc.c"
};
char data_02168760[4] = {
#include "data/main/data_02168760.inc.c"
};
char data_0216875c[4] = {
#include "data/main/data_0216875c.inc.c"
};
/* declared `u8 gCoPickOrder[CO_PICK_ORDER_LENGTH]` in build_co_pick_list.c: declared in src/game/main/menus/build_co_pick_list.c, with a type no header defines */
u8 gCoPickOrder[27] = {
#include "data/main/gCoPickOrder.inc.c"
};
u8 data_021687c4[28] = {
#include "data/main/data_021687c4.inc.c"
};
TaskScriptCmd data_021687e0[6] = {
#include "data/main/data_021687e0.inc.c"
};
TaskScriptCmd gCoNoPowerScript[8] = {
#include "data/main/gCoNoPowerScript.inc.c"
};
/* declared `Unk_02168850Row data_02168850[]` in func_020e3554.c: declared in src/game/main/func_020e3554.c, with a type no header defines */
void* data_02168850[27] = {
    &data_0216875c, &data_02168784, &data_02168760, &data_0216875c,
    &data_02168784, &data_02168760, &data_02168750, &data_021687a4,
    &data_021687a0, &data_0216879c, &data_02168798, &data_02168794,
    &data_02168790, &data_0216878c, &data_02168788, &data_02168754,
    &data_02168758, &data_02168780, &data_02168778, &data_02168774,
    &data_02168780, &data_02168770, &data_0216876c, &data_02168780,
    &data_02168768, &data_02168764, &data_0216877c};
