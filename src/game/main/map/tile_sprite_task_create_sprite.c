#include "types.h"
#include "game/screen_other_map.h"
#include "game/sprite_anim.h"
#include "game/tile_sprite_task.h"
#include "game/Unk_022a7c5c.h"

extern const u8 gTileSpriteFile[]; /* sprite file */

SpriteAnim* SpriteAnim_Create(const void* file, int a, int b);

/* Creates the task's sprite: on the 3D screen its graphics go where the
 * data_022a7c5c object says, otherwise it uses the tile in task->tile. */
void TileSpriteTask_CreateSprite(TileSpriteTask* task) {
  if (data_027c027c == data_027c0288.unk_00) {
    task->sprite = SpriteAnim_Create(gTileSpriteFile, 0, 30);
    task->sprite->tileBase = (u32)data_022a7c5c->unk_68;
    task->sprite->unk_28 = (u32)data_022a7c5c->unk_6c;
  } else {
    task->sprite = SpriteAnim_Create(gTileSpriteFile, 0, 2);
    task->sprite->tileBase = task->tile;
  }
}
