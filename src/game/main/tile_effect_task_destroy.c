#include "types.h"
#include "game/tile_effect_task.h"

void SpriteAnim_Free(void* handle);

void TileEffectTask_Destroy(TileEffectTask* task) {
  SpriteAnim_Free(task->sprite);
}
