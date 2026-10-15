#include "types.h"
#include "game/Unk_02080440.h"
#include "game/silo_missile_task.h"

void Task_ResumeScript(void* task);
void SpriteAnim_Free(void* handle);

/* Frame step of the launching missile: draws the sprite at its map position
 * minus the view scroll and moves it up 5 pixels. Once it is above the top of
 * the view it is freed and the script goes on. */
void SiloMissileLaunch_Update(SiloMissileTask* task) {
  SpriteAnim_Draw(task->sprite, task->x - MAP_STATE->unk_1c4->scrollX,
                  task->y - MAP_STATE->unk_1c4->scrollY);
  task->y -= 5;
  if (task->y - MAP_STATE->unk_1c4->scrollY >= 0) {
    return;
  }
  SpriteAnim_Free(task->sprite);
  Task_ResumeScript(task);
}
