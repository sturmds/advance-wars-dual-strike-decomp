#include "types.h"
#include "game/zoom_sprite_task.h"
#include "game/Unk_022a7c5c.h"

extern const u8 data_0214a69c[]; /* sprite file */

SpriteAnim* SpriteAnim_Create(const void* file, int a, int b);
void ZoomSprite_LoadGraphics(int screenX, int screenY, int animation,
                             void* sprite);

/* Script step that creates the sprite (3D on the 3D screen, 2D otherwise),
 * loads its graphics at the task position and starts the zoom at 2x. */
void ZoomSpriteTask_Init(ZoomSpriteTask* task) {
  if (data_027c027c == data_027c0288.unk_00) {
    task->sprite = SpriteAnim_Create(data_0214a69c, 1, 30);
    task->sprite->tileBase = (u32)data_022a7c5c->unk_68;
    task->sprite->unk_28 = (u32)data_022a7c5c->unk_6c;
  } else {
    task->sprite = SpriteAnim_Create(data_0214a69c, 1, 2);
    task->sprite->tileBase = 0x3194;
  }
  ZoomSprite_LoadGraphics(
      task->x - data_027c027c->unk_48[data_027c0284]->unk_1c4->scrollX,
      task->y - data_027c027c->unk_48[data_027c0284]->unk_1c4->scrollY,
      task->animation, task->sprite);
  task->scale = 0x200;
  task->holdFrames = -1;
}
