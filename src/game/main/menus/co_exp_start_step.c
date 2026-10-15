#include "types.h"
#include "game/co_exp.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0229fb30.h"
#include "game/win_lose.h"

BOOL Campaign_IsHardMode(void);
void CoExp_StartLevelUpTask(void);
void Task_ResumeScript(Task* task);
void CoExp_DrawScreen(CoExp* exp, int a, int b, int c);

#define FRAME(task) (*(s16*)&(task)->unk_4c)
#define SLIDE_FRAMES 16

/* Script task step, 16 frames, passing the frame count to CoExp_DrawScreen.
 * On the first frame: on normal-mode campaign maps up to 231 it clears
 * the second CO's levelUpFrame (as CoExp_InitScreen does), then starts
 * the level-up task. Resumes the script after the last frame. */
void CoExp_StartStep(Task* task) {
  if (FRAME(task) == 0) {
    if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode()) {
      if (MAP_STATE->unk_1c4->mapId <= 231) {
        ((u8*)&data_0229fb30)[0x5ad] = 0;
      }
    }
    CoExp_StartLevelUpTask();
  }
  CoExp_DrawScreen(
      (CoExp*)((u8*)&gMenuState + (u32) & ((MenuState*)0)->coExp.exp), 0, 0,
      FRAME(task));
  if (FRAME(task) != SLIDE_FRAMES) {
    FRAME(task)++;
    return;
  }
  FRAME(task) = 0;
  Task_ResumeScript(task);
}
