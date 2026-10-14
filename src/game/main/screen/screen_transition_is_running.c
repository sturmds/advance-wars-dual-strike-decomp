#include "types.h"
#include "game/task.h"

void* Task_FindByScript(const TaskScriptCmd* script);

void BlendFadeFromBlack_Start(void);
void BlendFadeFromBlack_Update(void);
void BlendFadeToBlack_Start(void);
void BlendFadeToBlack_Update(void);
void ScreenBlendDarken_Start(void);
void ScreenBlendDarken_Update(void);
void ScreenBlendLighten_Start(void);
void ScreenBlendLighten_Update(void);
void ScreenFadeFromWhiteSpeed_Start(void);
void ScreenFadeFromWhiteSpeed_Update(void);
void ScreenFadeToWhiteSpeed_Start(void);
void ScreenFadeToWhiteSpeed_Update(void);
void ScreenFlash_End(void);
void ScreenFlash_Init(void);
void ScreenFlash_UpdateFadeIn(void);
void ScreenFlash_UpdateFadeOut(void);
void ScreenFlash_UpdateHold(void);
void ScreenShake_End(void);
void ScreenShake_Init(void);
void ScreenShake_Update(void);
void ScreenWipe_EaseDownUpdate(void);
void ScreenWipe_EaseUpUpdate(void);
void ScreenWipe_GrowCornersStart(void);
void ScreenWipe_GrowCornersStartCurrent(void);
void ScreenWipe_GrowCornersUpdate(void);
void ScreenWipe_ShrinkCornersStart(void);
void ScreenWipe_ShrinkCornersStartCurrent(void);
void ScreenWipe_ShrinkCornersUpdate(void);
void WindowWipe_StartHide(void);
void WindowWipe_StartHideCurrent(void);
void WindowWipe_StartShow(void);
void WindowWipe_StartShowCurrent(void);
void func_02005a0c(void);

TaskScriptCmd data_021572e4[4] = {
#include "data/main/data_021572e4.inc.c"
};
TaskScriptCmd data_02157284[4] = {
#include "data/main/data_02157284.inc.c"
};
TaskScriptCmd data_02157264[4] = {
#include "data/main/data_02157264.inc.c"
};
TaskScriptCmd data_021572a4[4] = {
#include "data/main/data_021572a4.inc.c"
};
TaskScriptCmd data_021572c4[4] = {
#include "data/main/data_021572c4.inc.c"
};
TaskScriptCmd data_02157244[4] = {
#include "data/main/data_02157244.inc.c"
};
TaskScriptCmd data_02157224[4] = {
#include "data/main/data_02157224.inc.c"
};
TaskScriptCmd data_02157344[4] = {
#include "data/main/data_02157344.inc.c"
};
TaskScriptCmd data_02157324[4] = {
#include "data/main/data_02157324.inc.c"
};
TaskScriptCmd data_02157304[4] = {
#include "data/main/data_02157304.inc.c"
};
u32 data_02157364[11] = {
#include "data/main/data_02157364.inc.c"
};
TaskScriptCmd data_02157390[6] = {
#include "data/main/data_02157390.inc.c"
};
TaskScriptCmd data_021573c0[6] = {
#include "data/main/data_021573c0.inc.c"
};
TaskScriptCmd data_02157420[6] = {
#include "data/main/data_02157420.inc.c"
};
TaskScriptCmd gScreenShakeScript[6] = {
#include "data/main/gScreenShakeScript.inc.c"
};
TaskScriptCmd data_021573f0[6] = {
#include "data/main/data_021573f0.inc.c"
};
TaskScriptCmd data_02157480[8] = {
#include "data/main/data_02157480.inc.c"
};
u32 data_021574c0[66] = {
#include "data/main/data_021574c0.inc.c"
};

/* TRUE while any screen fade, blend or wipe task is running. */
BOOL ScreenTransition_IsRunning(void) {
  if (Task_FindByScript(data_02157264) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157244) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021573f0) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157390) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157420) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021573c0) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157304) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021572a4) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157284) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021572c4) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157344) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021572e4) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157224) != NULL) {
    return TRUE;
  }
  return Task_FindByScript(data_02157324) != NULL;
}
