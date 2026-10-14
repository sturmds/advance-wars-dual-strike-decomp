#include "types.h"
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
extern u8 gMapScrollScript[];

char data_0216898c[4] = {
#include "data/main/data_0216898c.inc.c"
};
char data_021689a4[4] = {
#include "data/main/data_021689a4.inc.c"
};
char data_021689a0[4] = {
#include "data/main/data_021689a0.inc.c"
};
char data_0216899c[4] = {
#include "data/main/data_0216899c.inc.c"
};
char data_02168998[4] = {
#include "data/main/data_02168998.inc.c"
};
char data_02168994[4] = {
#include "data/main/data_02168994.inc.c"
};
char data_02168990[4] = {
#include "data/main/data_02168990.inc.c"
};
TaskScriptCmd gUnitDropScript[14] = {
#include "data/main/gUnitDropScript.inc.c"
};
TaskScriptCmd gCoveringFireScript[14] = {
#include "data/main/gCoveringFireScript.inc.c"
};
