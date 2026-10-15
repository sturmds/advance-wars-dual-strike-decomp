#include "types.h"
#include "game/task.h"
#include "game/sprite_anim.h"

void Task_Destroy(Task* task);

/* Draws the task's sprite each frame. The task ends when the sprite has
 * finished and its slot is free again. Needed to match: one flag first holds
 * "slot is free", then the opposite. */
void SpriteAnimTask_Update(SpriteAnimTask* task) {
  BOOL slotFree;

  if (SpriteAnim_Draw(task->sprite, task->x, task->y)) {
    return;
  }
  slotFree = TRUE;
  if (task->sprite != NULL && task->sprite->data != 0) {
    slotFree = FALSE;
  }
  slotFree = (slotFree == FALSE); /* now: slot in use */
  if (slotFree) {
    return;
  }
  Task_Destroy((Task*)task);
}
