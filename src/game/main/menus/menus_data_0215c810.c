#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void LinkTask_WaitForPeerData(void);
void MapSelect_Draw(void);
void MapSelect_HandleKeys(void);
void MapSelect_Init(void);
void MapSelect_NextPage(void);
void MapSelect_NextSubStripKey(void);
void MapSelect_OnMapButton(void);
void MapSelect_PrevPage(void);
void MapSelect_PrevSubStripKey(void);
void MapSelect_Update(void);
void PaletteCycle_StopBothScreens(void);
void Screen1_DestroyTextTasks(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_0215c9a8[];
void func_0205e898(void);
void func_0205ec20(void);
void func_0205ec5c(void);
void func_02060f84(void);
void func_02064564(void);
void MapSelect_PickMap(void);
void MapSelect_Cancel(void);

char data_0215c810[] = {
#include "data/main/data_0215c810.inc.c"
};
u32 data_0215c818[] = {
#include "data/main/data_0215c818.inc.c"
};
/* (32, 0), previous list */
MenuButtonTemplate data_0215c918 = {
#include "data/main/data_0215c918.inc.c"
};
/* (224, 160), func_0205e898 */
MenuButtonTemplate data_0215c8c8 = {
#include "data/main/data_0215c8c8.inc.c"
};
/* (192, 32), MapSelect_NextSubStripKey */
MenuButtonTemplate data_0215c8f0 = {
#include "data/main/data_0215c8f0.inc.c"
};
/* declared `u8 data_0215c8a0[]` in map_select_add_map_buttons.c: relocation at data_0215c8a0+0x14 is not on a 4-byte field */
u32 data_0215c8a0[] = {
#include "data/main/data_0215c8a0.inc.c"
};
/* (224, 96), previous page */
MenuButtonTemplate data_0215c878 = {
#include "data/main/data_0215c878.inc.c"
};
/* (192, 0), next list */
MenuButtonTemplate data_0215c850 = {
#include "data/main/data_0215c850.inc.c"
};
/* (224, 128), MapSelect_NextPage */
MenuButtonTemplate data_0215c940 = {
#include "data/main/data_0215c940.inc.c"
};
/* (32, 32), MapSelect_PrevSubStripKey */
MenuButtonTemplate data_0215c828 = {
#include "data/main/data_0215c828.inc.c"
};
TaskScriptCmd data_0215c968[] = {
#include "data/main/data_0215c968.inc.c"
};
TaskScriptCmd data_0215c9a8[] = {
#include "data/main/data_0215c9a8.inc.c"
};
