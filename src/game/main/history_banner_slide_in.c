#include "types.h"
#include "game/task.h"
#include "game/math.h"

#define FRAME(task) (*(s16*)&(task)->unk_4c)

void Task_ResumeScript(Task* task);
void HistoryBanner_Draw(int x, int y);

/* Script task: slides the banner in from the right, x 320 to 128 over 16
 * frames, then resumes the script. */
void HistoryBanner_SlideIn(Task* task) {
  FRAME(task)++;
  HistoryBanner_Draw(Math_Ease(4, 192, 0, FRAME(task), 16) + 128, 96);
  if (FRAME(task) != 16) {
    return;
  }
  FRAME(task) = 0;
  Task_ResumeScript(task);
}
