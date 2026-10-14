#include "types.h"
#include "game/task.h"
#include "game/Unk_02169b20.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern Unk_02169b20 data_02169b24[];

void func_020f37d0(void) {
  Task_Create(data_02169b24[data_021694c0->mode].script, TASK_LIST(3));
}
