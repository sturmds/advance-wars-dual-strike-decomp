#include "types.h"
#include "game/task.h"

void LinkReactionTask_ClearOwnCooldown(void);
void LinkTextSyncTask_Reset(void);
void LinkTextSyncTask_Update(void);
void func_02080838(void);
void func_02080db8(void);
void func_02080e78(void);
void LinkReactionTask_Init(void);
void LinkReaction_Update(void);
void LinkReactionTask_Update(void);

char data_021604dc[5] = {
#include "data/main/data_021604dc.inc.c"
};
/* declared `u8 data_021604e4[]` in func_02080708.c: relocation at data_021604e4+0x14 is not on a 4-byte field */
u32 data_021604e4[10] = {
#include "data/main/data_021604e4.inc.c"
};
/* declared `u8 data_0216050c[]` in func_02080708.c: relocation at data_0216050c+0xc is not on a 4-byte field */
u32 data_0216050c[10] = {
#include "data/main/data_0216050c.inc.c"
};
TaskScriptCmd data_02160534[7] = {
#include "data/main/data_02160534.inc.c"
};
/* the drawable characters: A-Z, a-z, ". ", 0-9, "!?,:/&-'" */
u8 data_0216056c[73] = {
#include "data/main/data_0216056c.inc.c"
};
u8 data_021605b8[80] = {
#include "data/main/data_021605b8.inc.c"
};
