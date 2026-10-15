#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02168850Row.h"
#include "game/task.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PowerBonus_ApplyNextUnit(void);
void PowerHighlightTask_Update(void);
void Power_ResetScrollAndRefresh(void);
void func_020d5b0c(void);

char data_02168754[] = {
#include "data/main/data_02168754.inc.c"
};
char data_02168750[] = {
#include "data/main/data_02168750.inc.c"
};
char data_02168758[] = {
#include "data/main/data_02168758.inc.c"
};
u8 data_0216874c[] = {
#include "data/main/data_0216874c.inc.c"
};
char data_021687a4[] = {
#include "data/main/data_021687a4.inc.c"
};
char data_021687a0[] = {
#include "data/main/data_021687a0.inc.c"
};
char data_0216879c[] = {
#include "data/main/data_0216879c.inc.c"
};
char data_02168798[] = {
#include "data/main/data_02168798.inc.c"
};
char data_02168794[] = {
#include "data/main/data_02168794.inc.c"
};
char data_02168790[] = {
#include "data/main/data_02168790.inc.c"
};
char data_0216878c[] = {
#include "data/main/data_0216878c.inc.c"
};
char data_02168788[] = {
#include "data/main/data_02168788.inc.c"
};
char data_02168784[] = {
#include "data/main/data_02168784.inc.c"
};
char data_02168780[] = {
#include "data/main/data_02168780.inc.c"
};
char data_0216877c[] = {
#include "data/main/data_0216877c.inc.c"
};
char data_02168778[] = {
#include "data/main/data_02168778.inc.c"
};
char data_02168774[] = {
#include "data/main/data_02168774.inc.c"
};
char data_02168770[] = {
#include "data/main/data_02168770.inc.c"
};
char data_0216876c[] = {
#include "data/main/data_0216876c.inc.c"
};
char data_02168768[] = {
#include "data/main/data_02168768.inc.c"
};
char data_02168764[] = {
#include "data/main/data_02168764.inc.c"
};
char data_02168760[] = {
#include "data/main/data_02168760.inc.c"
};
char data_0216875c[] = {
#include "data/main/data_0216875c.inc.c"
};
/* declared `u8 gCoPickOrder[CO_PICK_ORDER_LENGTH]` in build_co_pick_list.c: declared in src/game/main/menus/build_co_pick_list.c, with a type no header defines */
u8 gCoPickOrder[] = {
#include "data/main/gCoPickOrder.inc.c"
};
/* COs in world map order, ended by CO_NONE. The end is written here, not
 * read from the ROM, so that COs can be added before it. */
u8 data_021687c4[] = {
#include "data/main/data_021687c4.inc.c"
    CO_NONE,
};
TaskScriptCmd data_021687e0[] = {
#include "data/main/data_021687e0.inc.c"
};
TaskScriptCmd gCoNoPowerScript[] = {
#include "data/main/gCoNoPowerScript.inc.c"
};
Unk_02168850Row data_02168850[] = {
    data_0216875c, data_02168784, data_02168760, data_0216875c, data_02168784,
    data_02168760, data_02168750, data_021687a4, data_021687a0, data_0216879c,
    data_02168798, data_02168794, data_02168790, data_0216878c, data_02168788,
    data_02168754, data_02168758, data_02168780, data_02168778, data_02168774,
    data_02168780, data_02168770, data_0216876c, data_02168780, data_02168768,
    data_02168764, data_0216877c};
