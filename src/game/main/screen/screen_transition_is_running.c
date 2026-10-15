#include "types.h"
#include "game/task.h"

void* Task_FindByScript(const TaskScriptCmd* script);

void BlendFadeFromBlack_Start(Task* task);
void BlendFadeFromBlack_Update(Task* task);
void BlendFadeToBlack_Start(Task* task);
void BlendFadeToBlack_Update(Task* task);
void ScreenBlendDarken_Start(Task* task);
void ScreenBlendDarken_Update(Task* task);
void ScreenBlendLighten_Start(Task* task);
void ScreenBlendLighten_Update(Task* task);
void ScreenFadeFromWhiteSpeed_Start(Task* task);
void ScreenFadeFromWhiteSpeed_Update(Task* task);
void ScreenFadeToWhiteSpeed_Start(Task* task);
void ScreenFadeToWhiteSpeed_Update(Task* task);
void ScreenFlash_End(void);
void ScreenFlash_Init(void);
void ScreenFlash_UpdateFadeIn(void);
void ScreenFlash_UpdateFadeOut(void);
void ScreenFlash_UpdateHold(void);
void ScreenShake_End(void);
void ScreenShake_Init(void);
void ScreenShake_Update(void);
void ScreenWipe_EaseDownUpdate(Task* task);
void ScreenWipe_EaseUpUpdate(Task* task);
void ScreenWipe_GrowCornersStart(Task* task);
void ScreenWipe_GrowCornersStartCurrent(Task* task);
void ScreenWipe_GrowCornersUpdate(Task* task);
void ScreenWipe_ShrinkCornersStart(Task* task);
void ScreenWipe_ShrinkCornersStartCurrent(Task* task);
void ScreenWipe_ShrinkCornersUpdate(Task* task);
void WindowWipe_StartHide(Task* task);
void WindowWipe_StartHideCurrent(Task* task);
void WindowWipe_StartShow(Task* task);
void WindowWipe_StartShowCurrent(Task* task);
void func_02005a0c(void);

TaskScriptCmd data_021572e4[] = {
#include "data/main/data_021572e4.inc.c"
};
TaskScriptCmd data_02157284[] = {
#include "data/main/data_02157284.inc.c"
};
TaskScriptCmd data_02157264[] = {
#include "data/main/data_02157264.inc.c"
};
TaskScriptCmd data_021572a4[] = {
#include "data/main/data_021572a4.inc.c"
};
TaskScriptCmd data_021572c4[] = {
#include "data/main/data_021572c4.inc.c"
};
TaskScriptCmd data_02157244[] = {
#include "data/main/data_02157244.inc.c"
};
TaskScriptCmd data_02157224[] = {
#include "data/main/data_02157224.inc.c"
};
TaskScriptCmd data_02157344[] = {
#include "data/main/data_02157344.inc.c"
};
TaskScriptCmd data_02157324[] = {
#include "data/main/data_02157324.inc.c"
};
TaskScriptCmd data_02157304[] = {
#include "data/main/data_02157304.inc.c"
};
u32 data_02157364[] = {
#include "data/main/data_02157364.inc.c"
};
TaskScriptCmd data_02157390[] = {
#include "data/main/data_02157390.inc.c"
};
TaskScriptCmd data_021573c0[] = {
#include "data/main/data_021573c0.inc.c"
};
TaskScriptCmd data_02157420[] = {
#include "data/main/data_02157420.inc.c"
};
TaskScriptCmd gScreenShakeScript[] = {
#include "data/main/gScreenShakeScript.inc.c"
};
TaskScriptCmd data_021573f0[] = {
#include "data/main/data_021573f0.inc.c"
};
TaskScriptCmd data_02157480[] = {
#include "data/main/data_02157480.inc.c"
};
u32 data_021574c0[] = {
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
