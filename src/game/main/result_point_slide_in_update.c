#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"
#include "game/math.h"

void ResultPoint_DrawRows(int x, int y, int arg);
void ResultPoint_DrawPanel(int x, int y, int arg);
void ResultPoint_DrawBar(int x, int y);
void Task_ResumeScript(ResultPointTask* task);

#define SLIDE_IN_FRAMES 24

/* Task update: slides the score rows, their bar and the points panel in
 * from x -160 to x 8 over SLIDE_IN_FRAMES frames, then resumes the script. */
void ResultPoint_SlideInUpdate(ResultPointTask* task) {
  int x = Math_Ease(4, -160, 8, task->frame, SLIDE_IN_FRAMES);

  ResultPoint_DrawRows(x, 16, -1);
  ResultPoint_DrawBar(x, 160);
  ResultPoint_DrawPanel(x, 160, -1);
  if (task->frame != SLIDE_IN_FRAMES) {
    task->frame++;
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
