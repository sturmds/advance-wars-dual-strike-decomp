#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba4.h"

void SpriteAnim_Free(void* handle);
void Mem_Free(void* block);

void Ov005_FreeStateWork(Ov005Task* task) {
  Unk_ov005_0236bba4* work = task->work;
  int i;

  for (i = 0; i < (int)ARRAY_COUNT(work->unk_390); i++) {
    SpriteAnim_Free(work->unk_390[i]);
  }
  Mem_Free(work);
}
