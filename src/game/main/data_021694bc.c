#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/screen_contexts.h"
#include "game/song.h"
#include "game/task.h"

void EffectScene_InitSprite(void);
void EffectScene_ScrollEaseInit(void);
void EffectScene_ScrollEaseStep(void);
void EffectScene_Update(void);
void EventScript_IsRunning(void);
void Map_ResetFrameCallbacks(void);
void MenuDepth_Pop(void);
void MenuDepth_PopBothScreens(void);
void MenuDepth_Push(void);
void NumberSpriteTask_SlideOut(void);
void NumberSpriteTask_Update(void);
void SceneMode_UpdateMode5(void);
void ScreenOverlays_Destroy(void);
void Screen_ClearLayers01(void);
void Screen_InitWithPlayerFrame(void);
void TimeDial_Reset(void);
void TimeDial_Update(void);
void TwoMaps_MergeOtherFront(void);
void TwoMaps_StartResultScript(void);
void UnitInfoPanel_Close(void);
extern u32 data_02169590[];
extern u32 data_021697b8[];
extern u32 data_02169818[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
extern u8 data_022a7b98[];
void func_020b97e8(void);
void func_020b99d4(void);
void func_020d3bf8(void);
void func_020f1af0(void);
void func_020f1be4(void);
void func_020f1cc4(void);
void func_020f1cd0(void);
void func_020f1d04(void);
void func_020f1da8(void);
void func_020f1de8(void);
void func_020f1ed0(void);
void func_020f1f38(void);
void func_020f2004(void);
void func_020f2014(void);
void func_020f28a8(void);
void func_020f2a18(void);
void func_020f3110(void);
void func_020f379c(void);
void func_020f37d0(void);
void func_020f3804(void);
void func_020f3858(void);
void func_020f38ac(void);
void func_020f3ec8(void);
void func_020f4628(void);
void ScreenOverlays_Create(void);
extern u8 data_0217fb08[];
void func_020f1ac4(void);
void func_020f1bb8(void);
void func_020f1c04(void);
void func_020f29ec(void);
void func_020f416c(void);
void func_020f43f4(void);
void func_020f4624(void);
void EffectScene_ShakeStep(void);
void SceneMode_Advance(void);

#pragma explicit_zero_data on
char data_021694c4[4] = {
#include "data/main/data_021694c4.inc.c"
};
Unk_021694c0* data_021694c0 = (Unk_021694c0*)&data_022a7b98;
u32 data_021694bc = {
#include "data/main/data_021694bc.inc.c"
};
char data_021694d8[5] = {
#include "data/main/data_021694d8.inc.c"
};
char data_021694c8[5] = {
#include "data/main/data_021694c8.inc.c"
};
char data_021694d0[5] = {
#include "data/main/data_021694d0.inc.c"
};
char data_021694e0[6] = {
#include "data/main/data_021694e0.inc.c"
};
char data_021694e8[6] = {
#include "data/main/data_021694e8.inc.c"
};
char data_021694f0[8] = {
#include "data/main/data_021694f0.inc.c"
};
char data_021694f8[9] = {
#include "data/main/data_021694f8.inc.c"
};
char data_02169504[10] = {
#include "data/main/data_02169504.inc.c"
};
char data_02169510[10] = {
#include "data/main/data_02169510.inc.c"
};
u32 data_0216951c[4] = {
#include "data/main/data_0216951c.inc.c"
};
u32 data_0216955c[4] = {
#include "data/main/data_0216955c.inc.c"
};
/* declared `ShakeOffsets gEffectSceneShakeOffsets` in effect_scene_shake_step.c: declared in src/game/main/effect_scene_shake_step.c, with a type no header defines */
u32 gEffectSceneShakeOffsets[4] = {
#include "data/main/gEffectSceneShakeOffsets.inc.c"
};
u32 data_0216956c[4] = {
#include "data/main/data_0216956c.inc.c"
};
u32 data_0216953c[4] = {
#include "data/main/data_0216953c.inc.c"
};
u32 data_0216952c[4] = {
#include "data/main/data_0216952c.inc.c"
};
u32 data_0216957c[5] = {
#include "data/main/data_0216957c.inc.c"
};
int* data_021695a4[5] = {(int*)&data_02169590, (int*)&data_0216951c,
                         (int*)&data_0216955c, (int*)&data_0216957c,
                         (int*)&data_0216957c};
u32 data_02169590[5] = {
#include "data/main/data_02169590.inc.c"
};
u32 data_02169738[6] = {
#include "data/main/data_02169738.inc.c"
};
u32 data_021696d8[6] = {
#include "data/main/data_021696d8.inc.c"
};
u32 data_021696f0[6] = {
#include "data/main/data_021696f0.inc.c"
};
/* event script per army colour, for a player out of it */
u32 data_02169708[6] = {
#include "data/main/data_02169708.inc.c"
};
u32 data_02169720[6] = {
#include "data/main/data_02169720.inc.c"
};
u32 data_02169750[6] = {
#include "data/main/data_02169750.inc.c"
};
u32 data_02169768[6] = {
#include "data/main/data_02169768.inc.c"
};
u32 data_02169780[6] = {
#include "data/main/data_02169780.inc.c"
};
u32 data_02169678[6] = {
#include "data/main/data_02169678.inc.c"
};
TaskScriptCmd data_021695b8[3] = {
#include "data/main/data_021695b8.inc.c"
};
u32 data_021695d0[6] = {
#include "data/main/data_021695d0.inc.c"
};
TaskScriptCmd data_021695e8[3] = {
#include "data/main/data_021695e8.inc.c"
};
u32 data_02169600[6] = {
#include "data/main/data_02169600.inc.c"
};
u32 data_02169618[6] = {
#include "data/main/data_02169618.inc.c"
};
u32 data_02169630[6] = {
#include "data/main/data_02169630.inc.c"
};
u32 data_02169648[6] = {
#include "data/main/data_02169648.inc.c"
};
u32 data_02169660[6] = {
#include "data/main/data_02169660.inc.c"
};
u32 data_02169690[6] = {
#include "data/main/data_02169690.inc.c"
};
u32 data_021696a8[6] = {
#include "data/main/data_021696a8.inc.c"
};
/* event script per army colour, for a player in the game */
u32 data_021696c0[6] = {
#include "data/main/data_021696c0.inc.c"
};
TaskScriptCmd data_021697f8[4] = {
#include "data/main/data_021697f8.inc.c"
};
u32 data_02169818[8] = {
#include "data/main/data_02169818.inc.c"
};
u32 data_02169838[8] = {
#include "data/main/data_02169838.inc.c"
};
TaskScriptCmd gNumberSpriteTaskScript[4] = {
#include "data/main/gNumberSpriteTaskScript.inc.c"
};
u32 data_02169798[8] = {
#include "data/main/data_02169798.inc.c"
};
u32 data_021697b8[8] = {
#include "data/main/data_021697b8.inc.c"
};
u32 data_021697d8[8] = {
#include "data/main/data_021697d8.inc.c"
};
TaskScriptCmd data_02169878[5] = {
#include "data/main/data_02169878.inc.c"
};
TaskScriptCmd data_021698a0[6] = {
#include "data/main/data_021698a0.inc.c"
};
TaskScriptCmd data_021698d0[6] = {
#include "data/main/data_021698d0.inc.c"
};
TaskScriptCmd data_02169900[6] = {
#include "data/main/data_02169900.inc.c"
};
TaskScriptCmd data_02169930[8] = {
#include "data/main/data_02169930.inc.c"
};
/* declared `u8 data_02169970[]` in screen_init_with_player_frame.c: relocation at data_02169970+0xc is not on a 4-byte field */
u32 data_02169970[20] = {
#include "data/main/data_02169970.inc.c"
};
/* declared `u8 gBmap085Layers[]` in gfx_load_bmap085.c: relocation at gBmap085Layers+0xc is not on a 4-byte field */
u32 gBmap085Layers[20] = {
#include "data/main/gBmap085Layers.inc.c"
};
/* declared `u8 data_02169a10[]` in func_020f4870.c: relocation at data_02169a10+0x4 is not on a 4-byte field */
u32 data_02169a10[20] = {
#include "data/main/data_02169a10.inc.c"
};
/* declared `u8 gBmap083Layers[]` in gfx_load_bmap083.c: relocation at gBmap083Layers+0xc is not on a 4-byte field */
u32 gBmap083Layers[20] = {
#include "data/main/gBmap083Layers.inc.c"
};
TaskScriptCmd data_02169ab0[14] = {
#include "data/main/data_02169ab0.inc.c"
};
/* declared `Unk_02169b20 data_02169b20[]` in func_020f379c.c: declared in src/game/main/func_020f379c.c, with a type no header defines */
void* data_02169b20[33] = {
    &data_02169690, &data_021696a8, &data_021694d0, &data_02169690,
    &data_021696a8, &data_021694f0, &data_021696d8, &data_021696f0,
    &data_02169510, &data_02169720, &data_02169738, &data_021694d8,
    &data_02169750, &data_02169768, &data_021694f8, &data_02169798,
    &data_02169660, &data_021694c8, &data_02169618, &data_021695d0,
    &data_021694e8, &data_02169600, &data_02169630, &data_02169504,
    &data_02169838, &data_02169780, &data_021694e0, &data_0216952c,
    &data_0216956c, &data_021694bc, &data_02169648, &data_0216953c,
    &data_021694c4};
s32 data_02169ba4[8][5] = {
#include "data/main/data_02169ba4.inc.c"
};
#pragma explicit_zero_data reset
