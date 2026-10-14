#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Sound_FadeOutSongOver(int length);

/* also declared in src/game/ov005/func_ov005_023659b8.c and 19 more */
Unk_ov005_0236bbac* data_ov005_0236bbac;

void Ov005SoundRoom_AllocWork(Ov005Task* task) {
  Unk_ov005_0236bbac* work;

  task->work = Mem_Alloc(sizeof(Unk_ov005_0236bbac));
  work = task->work;
  data_ov005_0236bbac = work;
  Mem_Zero32(0, work, sizeof(Unk_ov005_0236bbac));
  Sound_FadeOutSongOver(1);
}
