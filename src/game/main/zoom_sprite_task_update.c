#include "types.h"
#include "game/zoom_sprite_task.h"

/* Draws the sprite, then moves its scale halfway toward 1x (0x200, 0x180,
 * ... 0x104, then 0x100). */
void ZoomSpriteTask_Update(ZoomSpriteTask* task) {
  int next;

  ZoomSpriteTask_Draw(task);
  next = task->scale - (task->scale - 0x100) / 2;
  if (next - 0x100 == 4) {
    next = 0x100;
  }
  task->scale = next;
}
