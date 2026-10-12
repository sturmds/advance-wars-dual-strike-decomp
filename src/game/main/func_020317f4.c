#include "types.h"
#include "game/staggered_task.h"

void func_020318ac(int item);
void Task_ResumeScript(StaggeredTask* task);

/* Per-frame update. Every 16 frames it takes the lowest pending item, calls
 * func_020318ac for it and clears its bit. When no items are left it resumes
 * the task script. */
void func_020317f4(StaggeredTask* task) {
  int item;
  int bit;

  if (task->frame == 0) {
    item = 0;
    for (;;) {
      bit = 1 << item;
      if (task->pending & bit) {
        break;
      }
      item++;
    }
    func_020318ac(item);
    task->pending &= bit ^ -1; /* needed to match */
    task->itemsLeft--;
  }
  if (task->itemsLeft == 0) {
    Task_ResumeScript(task);
    return;
  }
  task->frame++;
  if (task->frame == 16) {
    task->frame = 0;
  }
}
