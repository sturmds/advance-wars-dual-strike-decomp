#include "types.h"
#include "game/task.h"

void Ov000_ClearRightPanelBox(void);
void PlayerPanels_Init(void);
void SecondScreen_UpdatePlayerPanels(void);

TaskScriptCmd data_ov000_0234eb64[] = {
#include "data/ov000/data_ov000_0234eb64.inc.c"
};
int data_ov000_0234eb84[][5] = {
#include "data/ov000/data_ov000_0234eb84.inc.c"
};
