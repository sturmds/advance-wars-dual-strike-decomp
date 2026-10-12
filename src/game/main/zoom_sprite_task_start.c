#include "types.h"
#include "game/zoom_sprite_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_021664b8[];

/* Starts a ZoomSpriteTask (ZoomSpriteTask_Init, _Update) on tile (tileX,
 * tileY). Variant 1 uses animation 4, any other variant animation 2. */
void ZoomSpriteTask_Start(int tileX, int tileY, int variant) {
  ZoomSpriteTask* task =
      (ZoomSpriteTask*)Task_Create(data_021664b8, TASK_LIST(3));

  task->x = tileX * 16 + 8;
  task->y = tileY * 16 + 16;
  switch (variant) {
    case 0:
    default:
      task->animation = 2;
      break;
    case 1:
      task->animation = 4;
      break;
  }
}
