#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"

extern Unk_02080440* data_027c027c;

void SpriteAnim_Free(void* sprite);
void Mem_Free(void* block);

/* Frees the sprite and both loaded files, and gives the slot back. */
void UnitShowTask_OnDestroy(UnitShowTask* task) {
  SpriteAnim_Free(task->sprite);
  Mem_Free(task->unk_44);
  Mem_Free(task->unk_48);
  data_027c027c->unk_58[task->slot] = 0;
}
