#include "types.h"
#include "game/event_script.h"
#include "game/song.h"
#include "game/task.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void SceneMode_IsChanging(void);
void SiloLaunch_PickTargetStep(void);
void TileEffectTask_StartMissileFall(void);
void TileEffectTask_StartMissileFallNoEffect(void);
void UnitMenu_Open(void);
void func_020eee70(void);
void func_020eee88(void);
void func_020eeec8(void);
void func_020eef10(void);
void func_020eef28(void);
void func_020eef40(void);
void func_020eef58(void);
void func_020ef110(void);
void func_020f20b4(void);
void func_020f20ec(void);
void func_020f20fc(void);
void func_020f225c(void);
extern u8 gMapScrollScript[];

/* "066" */
char data_021691c4[4] = {
#include "data/main/data_021691c4.inc.c"
};
/* "065" */
char data_021691c0[4] = {
#include "data/main/data_021691c0.inc.c"
};
/* "04d" */
char data_021691c8[4] = {
#include "data/main/data_021691c8.inc.c"
};
/* "0ba", the text's palette */
char data_021691bc[4] = {
#include "data/main/data_021691bc.inc.c"
};
/* "04c" */
char data_02169238[4] = {
#include "data/main/data_02169238.inc.c"
};
/* "04b" */
char data_02169234[4] = {
#include "data/main/data_02169234.inc.c"
};
/* "04a" */
char data_02169230[4] = {
#include "data/main/data_02169230.inc.c"
};
/* "049" */
char data_0216922c[4] = {
#include "data/main/data_0216922c.inc.c"
};
/* "048" */
char data_02169228[4] = {
#include "data/main/data_02169228.inc.c"
};
/* "047" */
char data_02169224[4] = {
#include "data/main/data_02169224.inc.c"
};
/* "046" */
char data_02169220[4] = {
#include "data/main/data_02169220.inc.c"
};
/* "045" */
char data_0216921c[4] = {
#include "data/main/data_0216921c.inc.c"
};
/* "044" */
char data_02169218[4] = {
#include "data/main/data_02169218.inc.c"
};
/* "05d" */
char data_02169214[4] = {
#include "data/main/data_02169214.inc.c"
};
/* "05c" */
char data_02169210[4] = {
#include "data/main/data_02169210.inc.c"
};
/* "05b" */
char data_0216920c[4] = {
#include "data/main/data_0216920c.inc.c"
};
/* "05a" */
char data_02169208[4] = {
#include "data/main/data_02169208.inc.c"
};
/* "059" */
char data_02169204[4] = {
#include "data/main/data_02169204.inc.c"
};
/* "058" */
char data_02169200[4] = {
#include "data/main/data_02169200.inc.c"
};
/* "063" */
char data_021691fc[4] = {
#include "data/main/data_021691fc.inc.c"
};
/* "062" */
char data_021691f8[4] = {
#include "data/main/data_021691f8.inc.c"
};
/* "061" */
char data_021691f4[4] = {
#include "data/main/data_021691f4.inc.c"
};
/* "057" */
char data_021691f0[4] = {
#include "data/main/data_021691f0.inc.c"
};
/* "056" */
char data_021691ec[4] = {
#include "data/main/data_021691ec.inc.c"
};
/* "055" */
char data_021691e8[4] = {
#include "data/main/data_021691e8.inc.c"
};
/* "054" */
char data_021691e4[4] = {
#include "data/main/data_021691e4.inc.c"
};
/* "053" */
char data_021691e0[4] = {
#include "data/main/data_021691e0.inc.c"
};
/* "052" */
char data_021691dc[4] = {
#include "data/main/data_021691dc.inc.c"
};
/* "051" */
char data_021691d8[4] = {
#include "data/main/data_021691d8.inc.c"
};
/* "050" */
char data_021691d4[4] = {
#include "data/main/data_021691d4.inc.c"
};
/* "04f" */
char data_021691d0[4] = {
#include "data/main/data_021691d0.inc.c"
};
/* "04e" */
char data_021691cc[4] = {
#include "data/main/data_021691cc.inc.c"
};
EventScriptCommand data_0216923c[5] = {
#include "data/main/data_0216923c.inc.c"
};
TaskScriptCmd data_0216928c[11] = {
#include "data/main/data_0216928c.inc.c"
};
TaskScriptCmd data_021692e4[12] = {
#include "data/main/data_021692e4.inc.c"
};
TaskScriptCmd data_02169344[15] = {
#include "data/main/data_02169344.inc.c"
};
TaskScriptCmd data_021693bc[19] = {
#include "data/main/data_021693bc.inc.c"
};
