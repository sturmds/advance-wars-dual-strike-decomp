#include "types.h"
#include "game/Unk_02168140.h"

void func_020f5878(s16 x, s16 y);

/* Passes the task's tile, moved 1 right and 2 down, to func_020f5878. Name
 * kept: the task's purpose is unknown. */
void func_020d9234(Unk_02168140Task* task) {
  func_020f5878(task->unk_2c + 1, task->unk_30 + 2);
}
