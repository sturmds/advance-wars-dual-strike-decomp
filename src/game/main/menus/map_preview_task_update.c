#include "types.h"
#include "game/task.h"

void func_02020848(void* dst);
void func_02020854(void* dst);
void MapPreview_DrawUnits(void* dst);

/* Update of the map preview task: redraws the preview in steps, one step on
 * frames 0, 1 and 22 of every 64. */
void MapPreviewTask_Update(Task* task) {
  int frame = task->unk_64;

  task->unk_64 = frame + 1;
  switch (frame & 63) {
    case 0:
      func_02020854(task->unk_4c);
      break;
    case 1:
      func_02020848(task->unk_4c);
      break;
    case 22:
      MapPreview_DrawUnits(task->unk_4c);
      break;
  }
}
