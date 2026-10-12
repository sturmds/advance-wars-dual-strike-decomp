#include "types.h"
#include "game/sprite_anim.h"

SpriteAnim3dTask* SpriteAnim3dTask_Start(const void* file, const VecFx32* pos,
                                         u32 tileBase, u32 unk28, int animation,
                                         int layer);

/* SpriteAnim3dTask_Start with SPRITE_ANIM_ALT_REGISTER set on the new sprite. */
SpriteAnim3dTask* SpriteAnim3dTask_StartAlt(const void* file,
                                            const VecFx32* pos, u32 tileBase,
                                            u32 unk28, int animation,
                                            int layer) {
  SpriteAnim3dTask* task =
      SpriteAnim3dTask_Start(file, pos, tileBase, unk28, animation, layer);

  task->sprite->flags |= SPRITE_ANIM_ALT_REGISTER;
  return task;
}
