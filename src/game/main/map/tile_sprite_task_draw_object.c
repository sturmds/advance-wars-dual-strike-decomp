#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/tile_sprite_task.h"
#include "game/sprite_anim.h"

void Task_ResumeScript(void* task);
int SpriteAnim_Draw3d(struct SpriteAnim* sprite, VecFx32* pos, VecFx32* rot);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Draws the task's sprite object at its tile: in 3D on the screen whose
 * state is data_027c0288.unk_00, otherwise in 2D against the map scroll.
 * The script resumes once the draw call returns 0 (taken to mean the
 * animation ended). */
void TileSpriteTask_DrawObject(TileSpriteTask* task) {
  VecFx32 pos;
  int drawn;

  if (data_027c027c == data_027c0288.unk_00) {
    *REG_MTX_PUSH = 0;
    pos.x = ((task->x * 16 + 8) << 13) / 256;
    pos.y = (task->y * 16 + 8) * 0x6000 / 0x300;
    pos.z = 8;
    drawn = SpriteAnim_Draw3d(task->sprite, &pos, NULL);
    *REG_MTX_POP = 1;
  } else {
    drawn = SpriteAnim_Draw(task->sprite,
                            (task->x * 16 - GAME->unk_1c4->scrollX + 8) & 0x1ff,
                            (task->y * 16 - GAME->unk_1c4->scrollY + 8) & 0xff);
  }
  if (drawn) {
    return;
  }
  Task_ResumeScript(task);
}
