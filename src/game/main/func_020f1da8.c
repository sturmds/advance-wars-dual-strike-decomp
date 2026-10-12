#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

void Sound_PlaySe(u32 soundId);

void func_020f1da8(Unk_021694c0Task* task) {
  Sound_PlaySe(182);
  task->frame = 0;
  task->x = data_021694c0->unk_24;
  task->y = data_021694c0->unk_28;
}
