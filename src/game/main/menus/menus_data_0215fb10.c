#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Menu_CancelOnBButton(void);
void StatusScreen_Init(void);
void StatusScreen_QueueSprites(void);
void func_02075520(void);
void func_020754d0(void);

char data_0215fb10[] = {
#include "data/main/data_0215fb10.inc.c"
};
char data_0215fb28[] = {
#include "data/main/data_0215fb28.inc.c"
};
MenuButtonTemplate data_0215fb40 = {
#include "data/main/data_0215fb40.inc.c"
};
TaskScriptCmd data_0215fb68[] = {
#include "data/main/data_0215fb68.inc.c"
};
