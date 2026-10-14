#include "types.h"
#include "game/task.h"

void LinkMapRecordSync_Finish(void);
void LinkMapRecordSync_Init(void);
void LinkMapRecordSync_Update(void);
void func_020824e4(void);
void func_020825a4(void);

char data_021606e4[7] = {
#include "data/main/data_021606e4.inc.c"
};
TaskScriptCmd gLinkMapRecordSyncScript[8] = {
#include "data/main/gLinkMapRecordSyncScript.inc.c"
};
