#include "types.h"
#include "game/task.h"
#include "game/sprite_anim.h"

extern const TaskScriptCmd data_02158850[];

SpriteAnim* SpriteAnim_Create(const void* file, u16 layer, u8 flags);
void SpriteAnim_SetAnimation(SpriteAnim* sprite, int animation);

/* Like SpriteAnimTask_Start, but for a sprite drawn in 3D space at *pos. Also
 * stores unk28 in the sprite and returns the new task. */
SpriteAnim3dTask* SpriteAnim3dTask_Start(const void* file, const VecFx32* pos,
                                         u32 tileBase, u32 unk28, int animation,
                                         int layer) {
  SpriteAnim* sprite;
  SpriteAnim3dTask* task;

  sprite = SpriteAnim_Create(file, layer, SPRITE_ANIM_HALFWORD_FILE);
  SpriteAnim_SetAnimation(sprite, animation);
  sprite->tileBase = tileBase;
  sprite->unk_28 = unk28;
  task = (SpriteAnim3dTask*)Task_Create(data_02158850, TASK_LIST(3));
  task->sprite = sprite;
  task->pos.x = pos->x;
  task->pos.y = pos->y;
  task->pos.z = pos->z;
  return task;
}
