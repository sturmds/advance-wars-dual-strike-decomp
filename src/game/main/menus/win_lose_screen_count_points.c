#include "types.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/win_lose.h"

void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(WinLoseTask* task);
void WinLoseScreen_DrawPicturesTwoSides(WinLoseTask* task);
void WinLoseScreen_DrawPointsBox(s32 added, s32 total, int popFrame);

/* Task update of the two-side win/lose screen, like PointsTally_Count:
 * after 30 frames moves two points per frame from pointsAdded into the total
 * pointsTotal, with a tick sound every other frame. A or a touch moves the
 * rest at once. The script resumes when pointsAdded is empty. */
void WinLoseScreen_CountPoints(WinLoseTask* task) {
  WinLoseScreen_DrawPicturesTwoSides(task);
  WinLoseScreen_DrawPointsBox(task->pointsAdded, task->pointsTotal, -1);
  if (task->pointsAdded == 0 && task->frame >= 30) {
    task->frame = 0;
    Task_ResumeScript(task);
    return;
  }
  task->frame++;
  if (task->frame >= 30) {
    task->pointsAdded -= 2;
    task->pointsTotal += 2;
    while (task->pointsAdded < 0) {
      task->pointsAdded++;
      task->pointsTotal--;
    }
    if (task->pointsTotal > WIN_LOSE_POINTS_MAX) {
      task->pointsTotal = WIN_LOSE_POINTS_MAX;
    }
    if ((task->frame & 1) == 0) {
      Sound_PlaySe(0x1000c);
    }
  }
  if ((gKeypad->pressed & PAD_BUTTON_A) ||
      (gTouch.phase == TOUCH_PRESSED && gTouch.active == 1)) {
    task->pointsTotal += task->pointsAdded;
    task->pointsAdded = 0;
    task->frame = 0;
    if (task->pointsTotal > WIN_LOSE_POINTS_MAX) {
      task->pointsTotal = WIN_LOSE_POINTS_MAX;
    }
    Task_ResumeScript(task);
  }
}
