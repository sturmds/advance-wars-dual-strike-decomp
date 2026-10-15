#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/event_script.h"
#include "game/falling_sprites.h"
#include "game/menu_button.h"
#include "game/screen_contexts.h"
#include "game/song.h"
#include "game/task.h"
#include "game/screen.h"

void BothScreens_FadeToWhiteUpdate(void);
void CoExp_AddCellsCentered(void);
void CoExp_AddCellsMainLeft(void);
void CoExp_AddCellsMainRight(void);
void CoExp_InitScreen(void);
void CoExp_LevelEventsStep(void);
void CoExp_LoadSpriteFiles(void);
void CoExp_SlideInStep(void);
void CoExp_SlideStep(void);
void CoExp_StartStep(void);
void CoExp_TallyStep(void);
void CoExp_WaitForConfirm(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void ResultScreen_CloseToWhite(void);
void ResultScreen_Confirm(void);
void ResultScreen_Init(void);
void ResultSprite_Hold(void);
void ResultSprite_PathIn(void);
void ResultSprite_PopIn(void);
void ResultSprite_ScrollLayer(void);
void ResultSprite_Shake(void);
void ResultSprite_SlideOut(void);
void ResultSprite_StartPointScript(void);
void ResultSprite_WaitForA(void);
void ResultSprite_WaitPointScript(void);
void ResultSprite_WaitPointSlide(void);
void SpriteShower_Init(void);
void SpriteShower_Update(void);
void TwoLineCaption_AnimateCorners(void);
void TwoLineCaption_Draw(void);
void TwoLineCaption_GrowInLetters(void);
void TwoLineCaption_SweepBars(void);
void WinQuote_Close(void);
void WinQuote_Init(void);
void WinQuote_SlideIn(void);
void WinQuote_WaitKeys(void);
void WinQuote_WaitText(void);
extern TaskScriptCmd data_02159f80[];
void func_020317f4(void);
void func_02031894(void);
void func_02031a48(void);
void func_02031f48(void);
void func_02031f54(void);
void func_020323f0(void);
void func_02032410(void);
void func_0203295c(void);
void func_02032984(void);
void func_020329c0(void);
void func_02032a70(void);
void func_02035708(void);
void func_0203570c(void);
void func_020369e4(void);
void func_020369f0(void);
void func_020d331c(void);
void func_02031dc0(void);
void func_02032298(void);
void CoExpTask_HoldSprite(void);
void func_02031bec(void);

FallingSpriteFrame data_02159c64[] = {
#include "data/main/data_02159c64.inc.c"
};
FallingSpriteFrame data_02159c50[] = {
#include "data/main/data_02159c50.inc.c"
};
TaskScriptCmd data_02159ca8[] = {
#include "data/main/data_02159ca8.inc.c"
};
TaskScriptCmd data_02159c78[] = {
#include "data/main/data_02159c78.inc.c"
};
TaskScriptCmd data_02159c90[] = {
#include "data/main/data_02159c90.inc.c"
};
TaskScriptCmd data_02159ce0[] = {
#include "data/main/data_02159ce0.inc.c"
};
TaskScriptCmd data_02159cc0[] = {
#include "data/main/data_02159cc0.inc.c"
};
MenuButtonTemplate data_02159d00 = {
#include "data/main/data_02159d00.inc.c"
};
MenuButtonTemplate data_02159d28 = {
#include "data/main/data_02159d28.inc.c"
};
TaskScriptCmd data_02159d50[] = {
#include "data/main/data_02159d50.inc.c"
};
TaskScriptCmd data_02159d78[] = {
#include "data/main/data_02159d78.inc.c"
};
TaskScriptCmd data_02159da8[] = {
#include "data/main/data_02159da8.inc.c"
};
EventScriptCommand data_02159e60[] = {
#include "data/main/data_02159e60.inc.c"
};
EventScriptCommand data_02159ea0[] = {
#include "data/main/data_02159ea0.inc.c"
};
EventScriptCommand data_02159de0[] = {
#include "data/main/data_02159de0.inc.c"
};
TaskScriptCmd data_02159e20[] = {
#include "data/main/data_02159e20.inc.c"
};
GfxLayer data_02159ee0[] = {
#include "data/main/data_02159ee0.inc.c"
};
GfxLayer data_02159f30[] = {
#include "data/main/data_02159f30.inc.c"
};
TaskScriptCmd data_02159f80[] = {
#include "data/main/data_02159f80.inc.c"
};
TaskScriptCmd data_02159ff8[] = {
#include "data/main/data_02159ff8.inc.c"
};
s16 gResultPathPoints[] = {
#include "data/main/gResultPathPoints.inc.c"
};
