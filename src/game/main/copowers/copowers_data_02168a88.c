#include "types.h"
#include "game/task.h"

void BlendFadeOut_Update(void);
void ExMachinaBolt_FadeIn(void);
void ExMachinaBolt_LoadGraphics(void);
void ExMachinaBolt_PlayAnim(void);
void ExMachinaBolt_ScrollPicture(void);
void ExMachinaBolt_StartFlash(void);
void ExMachina_PickTarget(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Power_ResetScrollAndRefresh(void);
void Screen_StartLayer0Blend(void);
void func_020e4700(void);
void func_020e4e7c(void);
void func_020e4e98(void);
extern u8 gMapScrollScript[];

TaskScriptCmd gExMachinaScript[] = {
#include "data/main/gExMachinaScript.inc.c"
};
TaskScriptCmd gExMachinaBoltScript[] = {
#include "data/main/gExMachinaBoltScript.inc.c"
};
