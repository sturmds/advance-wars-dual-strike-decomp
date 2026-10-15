#include "types.h"
#include "game/Unk_02080440.h"
#include "game/win_lose.h"

void Task_ResumeScript(WinLoseTask* task);
void WinLoseScreen_DrawPicturesTwoSides(WinLoseTask* task);
void func_0203b614(void);
void WinLoseScreen_DrawPointsBox(s32 added, s32 total, int popFrame);

/* Task update of the win/lose screen: draws the points with the pop
 * animation, frame counting up to scoreWidth. Then sets the frame that
 * WinLoseScreen_ClosePanel counts down from (12 in Survival, 10 otherwise),
 * calls func_0203b614 (adds a button) and resumes the script. */
void WinLoseScreen_PopScore(WinLoseTask* task) {
  WinLoseScreen_DrawPicturesTwoSides(task);
  WinLoseScreen_DrawPointsBox(task->pointsAdded, task->pointsTotal,
                              task->frame);
  if (task->frame == task->scoreWidth) {
    if (MAP_STATE->gameMode == GAME_MODE_SURVIVAL) {
      task->frame = 12;
    } else {
      task->frame = 10;
    }
    func_0203b614();
    Task_ResumeScript(task);
  } else {
    task->frame++;
  }
}
