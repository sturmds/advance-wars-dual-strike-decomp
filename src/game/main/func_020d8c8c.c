#include "types.h"
#include "game/Unk_020d8c8c.h"

void SpriteAnim_Free(u32* handle);

void func_020d8c8c(Unk_020d8c8c* owner) {
  if (owner->handle == NULL) {
    return;
  }
  SpriteAnim_Free(owner->handle);
}
