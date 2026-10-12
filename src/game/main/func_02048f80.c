#include "types.h"
#include "game/staggered_task.h"

void CoRankScreen_StartMarkerTask(int item);
void Task_ResumeScript(StaggeredTask* task);

void func_02048f80(StaggeredTask* task) {
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
    CoRankScreen_StartMarkerTask(item);
    task->pending &= bit ^ -1;
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
