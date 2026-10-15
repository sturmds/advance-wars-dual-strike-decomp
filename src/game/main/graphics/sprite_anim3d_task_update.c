#include "types.h"
#include "game/task.h"
#include "game/sprite_anim.h"
#include "game/zoom_sprite_task.h"

void Task_Destroy(Task* task);

/* Draws the task's sprite at its position each frame, as SpriteAnimTask_Update
 * does in 2D. The position is copied to the stack first. */
void SpriteAnim3dTask_Update(SpriteAnim3dTask* task) {
  BOOL slotFree;
  VecFx32 pos;

  pos.x = task->pos.x;
  pos.y = task->pos.y;
  pos.z = task->pos.z;
  if (SpriteAnim_Draw3d(task->sprite, &pos, 0)) {
    return;
  }
  slotFree = TRUE;
  if (task->sprite != NULL && task->sprite->data != 0) {
    slotFree = FALSE;
  }
  slotFree = (slotFree == FALSE);
  if (slotFree) {
    return;
  }
  Task_Destroy((Task*)task);
}
