#include "types.h"
#include "game/task.h"

void ScreenDarkenIn_Start(void);
void ScreenDarkenIn_Update(void);
void ScreenDarkenOut_Start(void);
void ScreenDarkenOut_Update(void);
void Screens_HideWithWindows(void);
void SubScreenLayer2_SlideInStart(void);
void SubScreenLayer2_SlideInUpdate(void);
void SubScreenLayers01_SlideInStart(void);
void SubScreen_CyclePaletteColor(void);
void TapPrompt_WaitForInput(void);
void TotalRankScreen_Init(void);
extern TaskScriptCmd data_0215fd64[];
void func_020772d4(void);
void SubScreenLayers01_SlideInUpdate(void);
void TapPrompt_Draw(void);
void SubScreenSprite_ZoomInUpdate(void);
void SubScreenSprite_ShakeUpdate(void);

u32 data_0215fcec[] = {
#include "data/main/data_0215fcec.inc.c"
};
TaskScriptCmd data_0215fcfc[] = {
#include "data/main/data_0215fcfc.inc.c"
};
u32 data_0215fd2c[] = {
#include "data/main/data_0215fd2c.inc.c"
};
TaskScriptCmd data_0215fd64[] = {
#include "data/main/data_0215fd64.inc.c"
};
