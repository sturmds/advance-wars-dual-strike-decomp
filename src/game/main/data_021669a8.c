#include "types.h"
#include "game/event_script.h"

void DailyRepair_Step(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
extern u8 data_02166798[];
extern u8 data_021667d8[];
void func_020c4018(void);
void func_020cce5c(void);
void DailySupply_Step(void);
void ServiceCostPopup_SetShown(void);
void func_020199a4(void);
void CurrentPlayer_ShowSavedCursor(void);
void func_020cce70(void);
extern u8 gMapScrollScript[];

/* 0x8 bytes no code names, before data_021669b0 */
u32 data_021669a8[] = {
#include "data/main/data_021669a8.inc.c"
};
u32 data_021669b0[] = {
#include "data/main/data_021669b0.inc.c"
};
/* declared `const u8 data_02166a18[]` in func_020cce44.c: relocation at data_02166a18+0x0 is not on a 4-byte field */
u32 data_02166a18[] = {
#include "data/main/data_02166a18.inc.c"
};
/* declared `u8 data_02166b08[]` in event_script_start_for_game_mode.c: relocation at data_02166b08+0x4 is not on a 4-byte field */
u32 data_02166b08[] = {
#include "data/main/data_02166b08.inc.c"
};
EventScriptCommand data_02166a88[] = {
#include "data/main/data_02166a88.inc.c"
};
/* declared `u8 data_02166b88[]` in event_script_start_for_game_mode.c: relocation at data_02166b88+0x4 is not on a 4-byte field */
u32 data_02166b88[] = {
#include "data/main/data_02166b88.inc.c"
};
u32 data_02166c08[] = {
#include "data/main/data_02166c08.inc.c"
};
