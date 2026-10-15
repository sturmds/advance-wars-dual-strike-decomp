#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/big_font.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void Screens_SetupLayer0AlphaBlend(void);
void Screens_ZoomInFadeUpdate(Task* task);
void TitleCall_HoldNumber(void);
void TitleCall_HoldSubLetters(void);
void TitleCall_InitNumber(void);
void TitleCall_InitSubLetters(void);
void TitleCall_NumberSlideIn(Task* task);
void TitleCall_ScrollWalls(Task* task);
void TitleCall_SetUpScreens(void);
void TitleCall_SpriteZoomIn(Task* task);
void TitleCall_StartSong(void);
void TitleCall_SubLettersGrowIn(Task* task);
extern TaskScriptCmd data_0215bc10[];
void func_02050c9c(void);
void func_0205102c(void);
void func_020517cc(void);

u8 data_0215bb60[] = {
#include "data/main/data_0215bb60.inc.c"
};
TaskScriptCmd data_0215bb68[] = {
#include "data/main/data_0215bb68.inc.c"
};
TaskScriptCmd data_0215bb88[] = {
#include "data/main/data_0215bb88.inc.c"
};
TaskScriptCmd data_0215bba8[] = {
#include "data/main/data_0215bba8.inc.c"
};
TaskScriptCmd data_0215bbc8[] = {
#include "data/main/data_0215bbc8.inc.c"
};
TaskScriptCmd data_0215bbe8[] = {
#include "data/main/data_0215bbe8.inc.c"
};
TaskScriptCmd data_0215bc10[] = {
#include "data/main/data_0215bc10.inc.c"
};
GfxLayer data_0215bc40[] = {
#include "data/main/data_0215bc40.inc.c"
};
GfxLayer data_0215bc90[] = {
#include "data/main/data_0215bc90.inc.c"
};
BigFontGlyph data_0215bce0[] = {
#include "data/main/data_0215bce0.inc.c"
};
