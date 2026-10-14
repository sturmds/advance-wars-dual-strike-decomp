#include "types.h"
#include "game/screen_contexts.h"
#include "game/song.h"
#include "game/task.h"

void Battle_SetScreensWhite(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PowerScreen_EaseOutBlend(void);
void PowerScreen_SwapMapWithSparks(void);
void ScreensFadeBothInFromWhite_Update(void);
void ScreensFadeBothToWhite_Update(void);
void SparkBurst_Init(void);
void SparkBurst_Update(void);
void SparkBurst_Wait(void);
void TagBreak_EndBlend(void);
void TagBreak_EndBlendWithSidePieces(void);
void TagBreak_FadeOutBlend(void);
void TagBreak_FadeToWhite(void);
void TagBreak_HoldName(void);
void TagBreak_LettersPopIn(void);
void TagBreak_LoadThunder(void);
void TagBreak_SetUpScreens(void);
void TagBreak_StartSparks(void);
void TagGauge_CountUp(void);
void TagGauge_Hold(void);
void TagGauge_Init(void);
void TagGauge_IsAnimating(void);
void TagGauge_PopIn(void);
void TagGauge_SlideIn(void);
void TagPowerScreen_FadeInFromBlack(void);
void TagPowerScreen_FadeToWhite(void);
void TagPowerScreen_HoldName(void);
void TagPowerScreen_SetUp(void);
void TagPowerScreen_StretchInName(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_02055e40(void);
void func_02055f84(void);
void func_02057eec(void);
void func_02057ef8(void);
void func_02059280(void);
void func_020593c8(void);
void func_020d340c(void);
void TagBreak_SlideInSidePieces(void);
void TagPowerScreen_ScrollBackground(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];
void TagBreak_FadeInBlend(void);
void TagBreak_ScrollFrame(void);

u32 data_0215c2bc[6] = {
#include "data/main/data_0215c2bc.inc.c"
};
u32 data_0215c2d4[10] = {
#include "data/main/data_0215c2d4.inc.c"
};
TaskScriptCmd data_0215c2fc[6] = {
#include "data/main/data_0215c2fc.inc.c"
};
/* script of the tag firepower gauge */
TaskScriptCmd gTagGaugeScript[6] = {
#include "data/main/gTagGaugeScript.inc.c"
};
/* declared `s8 gSparkBurstOffsets[SPARK_BURST_SPARKS * 2]` in spark_burst_update.c: declared as one element of 0x1 bytes, but takes 0x48 */
u32 gSparkBurstOffsets[18] = {
#include "data/main/gSparkBurstOffsets.inc.c"
};
/* declared `u8 data_0215c3f4[]` in tag_break_set_up_screens.c: relocation at data_0215c3f4+0xc is not on a 4-byte field */
u32 data_0215c3f4[20] = {
#include "data/main/data_0215c3f4.inc.c"
};
/* declared `u8 data_0215c3a4[]` in tag_break_set_up_screens.c: relocation at data_0215c3a4+0xc is not on a 4-byte field */
u32 data_0215c3a4[20] = {
#include "data/main/data_0215c3a4.inc.c"
};
TaskScriptCmd data_0215c444[22] = {
#include "data/main/data_0215c444.inc.c"
};
TaskScriptCmd data_0215c4f4[34] = {
#include "data/main/data_0215c4f4.inc.c"
};
