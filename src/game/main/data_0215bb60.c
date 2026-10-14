#include "types.h"
#include "game/big_font.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void Screens_SetupLayer0AlphaBlend(void);
void Screens_ZoomInFadeUpdate(void);
void TitleCall_HoldNumber(void);
void TitleCall_HoldSubLetters(void);
void TitleCall_InitNumber(void);
void TitleCall_InitSubLetters(void);
void TitleCall_NumberSlideIn(void);
void TitleCall_ScrollWalls(void);
void TitleCall_SetUpScreens(void);
void TitleCall_SpriteZoomIn(void);
void TitleCall_StartSong(void);
void TitleCall_SubLettersGrowIn(void);
extern TaskScriptCmd data_0215bc10[];
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_02050c9c(void);
void func_0205102c(void);
void func_020517cc(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

u8 data_0215bb60[8] = {
#include "data/main/data_0215bb60.inc.c"
};
TaskScriptCmd data_0215bb68[4] = {
#include "data/main/data_0215bb68.inc.c"
};
TaskScriptCmd data_0215bb88[4] = {
#include "data/main/data_0215bb88.inc.c"
};
TaskScriptCmd data_0215bba8[4] = {
#include "data/main/data_0215bba8.inc.c"
};
TaskScriptCmd data_0215bbc8[4] = {
#include "data/main/data_0215bbc8.inc.c"
};
TaskScriptCmd data_0215bbe8[5] = {
#include "data/main/data_0215bbe8.inc.c"
};
TaskScriptCmd data_0215bc10[6] = {
#include "data/main/data_0215bc10.inc.c"
};
/* declared `const u8 data_0215bc40[]` in title_call_set_up_screens.c: relocation at data_0215bc40+0xc is not on a 4-byte field */
u32 data_0215bc40[20] = {
#include "data/main/data_0215bc40.inc.c"
};
/* declared `const u8 data_0215bc90[]` in title_call_set_up_screens.c: relocation at data_0215bc90+0xc is not on a 4-byte field */
u32 data_0215bc90[20] = {
#include "data/main/data_0215bc90.inc.c"
};
BigFontGlyph data_0215bce0[65] = {
#include "data/main/data_0215bce0.inc.c"
};
