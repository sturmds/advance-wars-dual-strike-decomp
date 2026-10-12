#include "types.h"
#include "game/points_tally_task.h"
#include "game/keypad.h"
#include "game/touch.h"

void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(void* task);
void PointsTally_Draw(int x, int y, int added, int total, int popFrame);

/* Moves two points per frame from added into total, with a tick sound
 * every other frame. A or a touch moves the rest at once. The script
 * resumes when added is empty. */
void PointsTally_Count(PointsTallyTask* task) {
  PointsTally_Draw(56, 88, task->added, task->total, -1);
  task->frame++;
  if (task->added == 0) {
    task->frame = 0;
    Task_ResumeScript(task);
    return;
  }
  task->added -= 2;
  task->total += 2;
  while (task->added < 0) {
    task->added++;
    task->total--;
  }
  if (task->total > POINTS_TALLY_MAX) {
    task->total = POINTS_TALLY_MAX;
  }
  if ((task->frame & 1) == 0) {
    Sound_PlaySe(0x1000c);
  }
  if ((gKeypad->pressed & PAD_BUTTON_A) ||
      (gTouch.phase == TOUCH_PRESSED && gTouch.active == 1)) {
    task->total += task->added;
    task->added = 0;
    task->frame = 0;
    if (task->total > POINTS_TALLY_MAX) {
      task->total = POINTS_TALLY_MAX;
    }
    Task_ResumeScript(task);
  }
}
