#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

void SpriteAnim_Free(void* handle);
void Mem_Free(void* block);
void Ov005SoundRoom_DestroyMusicTask(void);

void Ov005SoundRoom_FreeWork(Ov005Task* task) {
  Unk_ov005_0236bbac* work = task->work;

  Ov005SoundRoom_DestroyMusicTask();
  SpriteAnim_Free(work->mainObjects[0]);
  SpriteAnim_Free(work->mainObjects[1]);
  SpriteAnim_Free(work->mainObjects[2]);
  SpriteAnim_Free(work->mainObjects[3]);
  SpriteAnim_Free(work->subObjects[0]);
  SpriteAnim_Free(work->subObjects[1]);
  SpriteAnim_Free(work->subObjects[2]);
  SpriteAnim_Free(work->subObjects[3]);
  SpriteAnim_Free(work->subObjects[4]);
  Mem_Free(work->spriteFileA);
  Mem_Free(work->spriteFileB);
  Mem_Free(work);
}
