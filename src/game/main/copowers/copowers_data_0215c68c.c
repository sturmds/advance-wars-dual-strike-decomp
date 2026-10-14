#include "types.h"
#include "game/screen_contexts.h"
#include "game/song.h"
#include "game/task.h"

void CoSwapScreen_SetUp(void);
void CoSwap_SlideInCos(void);
void CoSwap_SlideOutCos(void);
void CoSwap_StartBlend(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_0205c02c(void);
void func_0205c09c(void);
void func_0205c82c(void);
void func_0205cd00(void);
void func_0205cd74(void);
void func_020b97e8(void);
void func_020d3310(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

/* "/ohashi/res_tagchange" */
char data_0215c68c[22] = {
#include "data/main/data_0215c68c.inc.c"
};
/* "/ohashi/res_tagbreak_thunder" */
char gTagBreakThunderPath[29] = {
#include "data/main/gTagBreakThunderPath.inc.c"
};
/* declared `u8 data_0215c6c4[]` in co_swap_screen_set_up.c: relocation at data_0215c6c4+0xc is not on a 4-byte field */
u32 data_0215c6c4[20] = {
#include "data/main/data_0215c6c4.inc.c"
};
/* declared `u8 data_0215c714[]` in co_swap_screen_set_up.c: relocation at data_0215c714+0xc is not on a 4-byte field */
u32 data_0215c714[20] = {
#include "data/main/data_0215c714.inc.c"
};
TaskScriptCmd data_0215c764[18] = {
#include "data/main/data_0215c764.inc.c"
};
