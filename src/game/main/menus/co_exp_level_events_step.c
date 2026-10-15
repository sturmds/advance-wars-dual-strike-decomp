#include "types.h"
#include "game/co_exp.h"
#include "game/menu_state.h"
#include "game/task.h"

BOOL Menu_IsBusy(void);
BOOL EventScript_IsRunning(void);
void CoExp_CheckFirstLevelUp(void);
void CoExp_CheckLevel10(void);
void CoExp_CheckMaxLevel(void);
void Task_ResumeScript(Task* task);
void func_02032a9c(void);
void CoExp_DrawScreen(CoExp* exp, int a, int b, int c);

/* Task step: redraws the CO cells, then plays the level-up event scripts
 * one at a time (first level-up, level 10, max level), waiting for each to
 * end. When none is left, func_02032a9c adds its menu button and the
 * script resumes. */
void CoExp_LevelEventsStep(Task* task) {
  CoExp_DrawScreen(
      (CoExp*)((u8*)&gMenuState + (u32) & ((MenuState*)0)->coExp.exp), 0, 0,
      -1);
  if (Menu_IsBusy() == TRUE) {
    return;
  }
  CoExp_CheckFirstLevelUp();
  if (EventScript_IsRunning()) {
    return;
  }
  CoExp_CheckLevel10();
  if (EventScript_IsRunning()) {
    return;
  }
  CoExp_CheckMaxLevel();
  if (EventScript_IsRunning()) {
    return;
  }
  func_02032a9c();
  Task_ResumeScript(task);
}
