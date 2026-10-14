#include "types.h"
#include "game/screen_contexts.h"
#include "game/song.h"
#include "game/task.h"

void CoPowerScreen_GrowOutName(void);
void CoPowerScreen_HoldName(void);
void CoPowerScreen_ScrollBackground(void);
void CoPowerScreen_SetUp(void);
void CoPowerScreen_ShrinkInName(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void ScreenFadeInFromBlack_Update(void);
void ScreenFadeInFromWhite_Update(void);
void ScreenFadeSprites_Update(void);
void ScreenFadeToBlackSlow_Update(void);
void ScreenFadeToBlack_Update(void);
void SuperBreak_BlendUpdate(void);
void SuperBreak_BounceBanner(void);
void SuperBreak_FadeFromWhite(void);
void SuperBreak_FadeToWhite(void);
void SuperBreak_FlashUpdate(void);
void SuperBreak_HoldFrame(void);
void SuperBreak_HoldName(void);
void SuperBreak_LettersPopInUpdate(void);
void SuperBreak_LoadBanner(void);
void SuperBreak_ScrollBackgrounds(void);
void SuperBreak_SetUpBackgroundBlend(void);
void SuperBreak_SetUpScreen(void);
void SuperBreak_SlideInBanner(void);
void SuperBreak_SlideInFrame(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_02055074(void);
void func_02055c88(void);
void func_020b97e8(void);
void func_020d3310(void);
void SuperBreak_EaseInBackground(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

u32 data_0215c018[4] = {
#include "data/main/data_0215c018.inc.c"
};
u32 data_0215c028[4] = {
#include "data/main/data_0215c028.inc.c"
};
u32 data_0215c038[6] = {
#include "data/main/data_0215c038.inc.c"
};
/* declared `u8 data_0215c0a0[]` in co_power_screen_set_up.c: relocation at data_0215c0a0+0xc is not on a 4-byte field */
u32 data_0215c0a0[20] = {
#include "data/main/data_0215c0a0.inc.c"
};
/* declared `u8 data_0215c050[]` in co_power_screen_set_up.c: relocation at data_0215c050+0xc is not on a 4-byte field */
u32 data_0215c050[20] = {
#include "data/main/data_0215c050.inc.c"
};
TaskScriptCmd data_0215c0f0[21] = {
#include "data/main/data_0215c0f0.inc.c"
};
TaskScriptCmd data_0215c198[26] = {
#include "data/main/data_0215c198.inc.c"
};
