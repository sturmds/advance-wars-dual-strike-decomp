#include "types.h"
#include "game/task.h"

void Ov000_ClearRightPanelBox(void);
extern char data_ov000_023504d0[];
void PlayerPanels_Init(void);
void SecondScreen_UpdatePlayerPanels(void);

/* declared `u8 data_ov000_0234eb60[]` in ov000_data_022f6bd8.c: relocation at data_ov000_0234eb60+0x0 is not on a 4-byte field */
void* data_ov000_0234eb60 = &data_ov000_023504d0;
TaskScriptCmd data_ov000_0234eb64[] = {
#include "data/ov000/data_ov000_0234eb64.inc.c"
};
int data_ov000_0234eb84[][5] = {
#include "data/ov000/data_ov000_0234eb84.inc.c"
};
