#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void UnitList_Close(void);
void UnitList_Destroy(void);
void UnitList_Draw(void);
void UnitList_HandleKeys(void);
void UnitList_Init(void);
void UnitList_OnSortButton(void);
void UnitList_ScrollToPickedUnit(void);
void UnitList_UpdateDownButton(void);
void UnitList_UpdateUpButton(void);
void UnitList_PickRow(void);
void func_02073af0(void);

char gLinkCommonResPath[22] = {
#include "data/main/gLinkCommonResPath.inc.c"
};
MenuButtonTemplate data_0215fa80 = {
#include "data/main/data_0215fa80.inc.c"
};
MenuButtonTemplate data_0215f9b8 = {
#include "data/main/data_0215f9b8.inc.c"
};
MenuButtonTemplate data_0215fa30 = {
#include "data/main/data_0215fa30.inc.c"
};
MenuButtonTemplate data_0215fa08 = {
#include "data/main/data_0215fa08.inc.c"
};
MenuButtonTemplate data_0215f9e0 = {
#include "data/main/data_0215f9e0.inc.c"
};
MenuButtonTemplate data_0215fa58 = {
#include "data/main/data_0215fa58.inc.c"
};
MenuButtonTemplate data_0215f990 = {
#include "data/main/data_0215f990.inc.c"
};
MenuButtonTemplate data_0215faa8 = {
#include "data/main/data_0215faa8.inc.c"
};
TaskScriptCmd data_0215fad0[8] = {
#include "data/main/data_0215fad0.inc.c"
};
