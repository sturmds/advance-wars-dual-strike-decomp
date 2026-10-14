#include "types.h"
#include "game/tile_offset.h"
#include "game/script_slot.h"
#include "game/event_script.h"

void DailyRepair_Step(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
extern EventScriptCommand data_02166798[];
extern EventScriptCommand data_021667d8[];
void func_020c4018(void);
void func_020cce5c(void);
void DailySupply_Step(void);
void ServiceCostPopup_SetShown(void);
void func_020199a4(void);
void CurrentPlayer_ShowSavedCursor(void);
void func_020cce70(void);
extern ScriptCommand gMapScrollScript[];

/* 0x8 bytes no code names, before data_021669b0 */
u32 data_021669a8[] = {
#include "data/main/data_021669a8.inc.c"
};
TileOffset data_021669b0[] = {
#include "data/main/data_021669b0.inc.c"
};
ScriptCommand data_02166a18[] = {
#include "data/main/data_02166a18.inc.c"
};
EventScriptCommand data_02166b08[] = {
#include "data/main/data_02166b08.inc.c"
};
EventScriptCommand data_02166a88[] = {
#include "data/main/data_02166a88.inc.c"
};
EventScriptCommand data_02166b88[] = {
#include "data/main/data_02166b88.inc.c"
};
TileOffset data_02166c08[] = {
#include "data/main/data_02166c08.inc.c"
};
