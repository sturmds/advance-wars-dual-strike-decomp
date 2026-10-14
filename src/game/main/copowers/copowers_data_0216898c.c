#include "types.h"
#include "game/script_slot.h"
#include "game/task.h"

void CoveringFire_PickFundsSite(void);
void CoveringFire_PickHpSite(void);
void CoveringFire_PickInfantrySite(void);
void CoveringFire_StartBlast(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Power_ResetScrollAndRefresh(void);
void UnitDrop_FindNextCity(void);
void UnitDrop_SpawnUnit(void);
void UnitDrop_Start(void);
void func_020d5b0c(void);
extern ScriptCommand gMapScrollScript[];

char data_0216898c[] = {
#include "data/main/data_0216898c.inc.c"
};
char data_021689a4[] = {
#include "data/main/data_021689a4.inc.c"
};
char data_021689a0[] = {
#include "data/main/data_021689a0.inc.c"
};
char data_0216899c[] = {
#include "data/main/data_0216899c.inc.c"
};
char data_02168998[] = {
#include "data/main/data_02168998.inc.c"
};
char data_02168994[] = {
#include "data/main/data_02168994.inc.c"
};
char data_02168990[] = {
#include "data/main/data_02168990.inc.c"
};
TaskScriptCmd gUnitDropScript[] = {
#include "data/main/gUnitDropScript.inc.c"
};
TaskScriptCmd gCoveringFireScript[] = {
#include "data/main/gCoveringFireScript.inc.c"
};
