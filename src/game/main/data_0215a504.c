#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void Game_IsWarRoom(void);
void ResultStats_BlendInStep(void);
void ResultStats_FadeInStep(void);
void ResultStats_Init(void);
void ResultStats_SlideIn(void);
void ResultStats_Update(void);
void ResultStats_WaitForClose(void);
void Wallpaper_DestroyTasks(void);
void WinLoseScreen_BlendIn(void);
void WinLoseScreen_ClosePanel(void);
void WinLoseScreen_CountPoints(void);
void WinLoseScreen_FadeInFromWhite(void);
void WinLoseScreen_FadeOutLayer2(void);
void WinLoseScreen_Init(void);
void WinLoseScreen_OpenPanel(void);
void WinLoseScreen_PopScore(void);
void WinLoseScreen_ShowSurvivalStats(void);
void WinLoseScreen_SlideInPicturesTwoSides(void);
void WinLoseScreen_SlideOutPictures(void);
void WinLoseScreen_SlideOutPicturesTwoSides(void);
void WinLoseScreen_WaitForKey(void);
extern TaskScriptCmd data_0215a690[];
void func_0202686c(void);
void func_0203b634(void);
void func_0203b640(void);
void func_0203b64c(void);
void func_0203b658(void);
void func_0203b664(void);
void func_0203b670(void);
void func_0203b67c(void);
void func_020d42b0(void);
void func_020d42e4(void);

u32 data_0215a504[6] = {
#include "data/main/data_0215a504.inc.c"
};
/* unit types, 7 rows of 4; 0 leaves a cell empty */
u8 data_0215a51c[28] = {
#include "data/main/data_0215a51c.inc.c"
};
MenuButtonTemplate data_0215a5d8 = {
#include "data/main/data_0215a5d8.inc.c"
};
MenuButtonTemplate data_0215a5b0 = {
#include "data/main/data_0215a5b0.inc.c"
};
MenuButtonTemplate data_0215a588 = {
#include "data/main/data_0215a588.inc.c"
};
MenuButtonTemplate data_0215a560 = {
#include "data/main/data_0215a560.inc.c"
};
MenuButtonTemplate data_0215a600 = {
#include "data/main/data_0215a600.inc.c"
};
MenuButtonTemplate data_0215a538 = {
#include "data/main/data_0215a538.inc.c"
};
TaskScriptCmd data_0215a628[13] = {
#include "data/main/data_0215a628.inc.c"
};
TaskScriptCmd data_0215a690[26] = {
#include "data/main/data_0215a690.inc.c"
};
