#include "types.h"
#include "game/Unk_02080440.h"
#include "game/silo_missile_task.h"
#include "game/task.h"
#include "game/screen.h"

void Task_ResumeScript(void* task);
void SpriteAnim_Free(void* handle);

/* Frame step of the falling missile: draws the sprite at its map position
 * minus the view scroll and moves it down 8 pixels. Past the target row it
 * is freed, the screen shakes and flashes, and the script goes on. */
void SiloMissileFall_Update(SiloMissileTask* task) {
  SpriteAnim_Draw(task->sprite, task->pixelX - MAP_STATE->unk_1c4->scrollX,
                  task->fallY - MAP_STATE->unk_1c4->scrollY);
  task->fallY += 8;
  if (task->fallY <= task->y * 16) {
    return;
  }
  ScreenShake_Start(2, 60, NULL);
  ScreenFlash_Start(4, 0, 2, NULL);
  SpriteAnim_Free(task->sprite);
  Task_ResumeScript(task);
}
