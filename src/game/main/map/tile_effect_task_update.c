#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/tile_effect_task.h"
#include "sdk/gx.h"

typedef struct Vec3 {
  int x;
  int y;
  int z;
} Vec3;

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

void Task_ResumeScript(TileEffectTask* task);
void MapSpriteTask_PlayKindEffects(TileEffectTask* task);
int SpriteAnim_Draw(void* sprite, int x, int y);
int SpriteAnim_Draw3d(void* sprite, Vec3* pos, Vec3* rot);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Each frame: steps the effect (MapSpriteTask_PlayKindEffects), then draws its sprite on
 * the tile, in 3D on the first screen and as a 2D sprite against the map
 * scroll on the other. The script resumes once the sprite is missing or
 * its draw call returns 0 (taken to mean the animation ended). */
void TileEffectTask_Update(TileEffectTask* task) {
  Vec3 pos;

  if (task->sprite == NULL) {
    Task_ResumeScript(task);
    return;
  }
  MapSpriteTask_PlayKindEffects(task);
  if (data_027c027c == data_027c0288.unk_00) {
    *REG_MTX_PUSH = 0;
    pos.x = ((task->x * 16 + 8) << 13) / 256;
    pos.y = (task->y * 16 + 16) * 0x6000 / 0x300;
    pos.z = 8;
    if (!SpriteAnim_Draw3d(task->sprite, &pos, NULL)) {
      Task_ResumeScript(task);
    }
    *REG_MTX_POP = 1;
  } else {
    if (!SpriteAnim_Draw(task->sprite,
                         task->x * 16 + 8 - GAME->unk_1c4->scrollX,
                         task->y * 16 + 16 - GAME->unk_1c4->scrollY)) {
      Task_ResumeScript(task);
    }
  }
}
