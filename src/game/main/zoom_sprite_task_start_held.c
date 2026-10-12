#include "types.h"
#include "game/zoom_sprite_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_021665d0[];

void func_020f5888(s16 x, s16 y);

/* Starts a ZoomSpriteTask whose script plays a sound and holds the sprite at
 * 1x before it resumes (ZoomSpriteTask_InitWithSound, _UpdateThenResume) on
 * tile (tileX, tileY), animation 0. Also calls func_020f5888 for the tile. */
void ZoomSpriteTask_StartHeld(int tileX, int tileY) {
  ZoomSpriteTask* task =
      (ZoomSpriteTask*)Task_Create(data_021665d0, TASK_LIST(3));

  task->x = tileX * 16 + 8;
  task->y = tileY * 16 + 16;
  task->animation = 0;
  func_020f5888(tileX, tileY);
}
