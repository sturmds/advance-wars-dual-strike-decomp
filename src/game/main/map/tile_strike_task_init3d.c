#include "types.h"
#include "game/tile_strike_task.h"

extern u8 data_02148eec[];

struct SpriteAnim* BmapSpriteAnim_Create(void* file, int a);

/* Makes the sprite at the tile, 100 units up. */
void TileStrikeTask_Init3d(TileStrikeTask* task) {
  task->sprite = BmapSpriteAnim_Create(data_02148eec, 1);
  task->x = task->tileX * 16 + 8;
  task->y = task->tileY * 16 + 8;
  task->height = 100;
}
