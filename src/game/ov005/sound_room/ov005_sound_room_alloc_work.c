#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

#define OV005_SOUND_ROOM_WORK_SIZE 0x1360

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Sound_FadeOutSongOver(int length);

/* also declared in src/game/ov005/func_ov005_023659b8.c and 19 more */
Unk_ov005_0236bbac* data_ov005_0236bbac;

void Ov005SoundRoom_AllocWork(Ov005Task* task) {
  Unk_ov005_0236bbac* work;

  task->work = Mem_Alloc(OV005_SOUND_ROOM_WORK_SIZE);
  work = task->work;
  data_ov005_0236bbac = work;
  Mem_Zero32(0, work, OV005_SOUND_ROOM_WORK_SIZE);
  Sound_FadeOutSongOver(1);
}
