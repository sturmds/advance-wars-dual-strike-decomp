#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

void func_020f1bb8(Unk_021694c0Task* task) {
  task->x = data_021694c0->unk_24;
  task->y = data_021694c0->unk_28;
  task->frame = 0;
}
