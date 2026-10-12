#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSpriteTask.h"

void Task_Destroy(void* task);

/* Destroys the cell's shadow sprite once it is done, otherwise moves it with
 * the cell. The second NULL check is needed to match. */
void BattleCellTask_UpdateShadow(BattleCellTask* task) {
  BattleSpriteTask* shadow;

  if (!task->hasShadow) {
    return;
  }
  shadow = task->shadow;
  if (shadow == NULL) {
    return;
  }
  if (shadow->unk_5d == 0) {
    if (shadow != NULL) {
      Task_Destroy(shadow);
      task->shadow = NULL;
    }
    task->hasShadow = 0;
    return;
  }
  task->shadow->unk_48 = BattleCellTask_GetOffsetY(task);
  task->shadow->unk_4a = task->angle + BattleCellTask_GetOffsetX(task);
}
