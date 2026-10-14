#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void HelpBand_Stop(void);
void ReactionTextMenu_DrawScreen(void);
void ReactionTextMenu_LoadIntoEntry(void);
void ReactionTextMenu_SetUpScreens(void);
void ReactionTextMenu_StartEventWhenDone(void);
void ReactionTextMenu_StoreTyped(void);
void Screen1_DestroyTextTasks(void);
void Wallpaper_DestroyTasks(void);
void func_02075eb4(void);
void func_02075f60(void);
void func_02075fa4(void);
void func_020760e4(void);
void func_02075f54(void);
void func_020760e0(void);
void func_02075e84(void);

/* status screen background file */
char data_0215fba0[] = {
#include "data/main/data_0215fba0.inc.c"
};
MenuButtonTemplate data_0215fbbc = {
#include "data/main/data_0215fbbc.inc.c"
};
MenuButtonTemplate data_0215fbe4 = {
#include "data/main/data_0215fbe4.inc.c"
};
/* declared `u8 data_0215fc0c[]` in func_02075fa4.c: relocation at data_0215fc0c+0xc is not on a 4-byte field */
u32 data_0215fc0c[] = {
#include "data/main/data_0215fc0c.inc.c"
};
TaskScriptCmd data_0215fc44[] = {
#include "data/main/data_0215fc44.inc.c"
};
TaskScriptCmd data_0215fc8c[] = {
#include "data/main/data_0215fc8c.inc.c"
};
