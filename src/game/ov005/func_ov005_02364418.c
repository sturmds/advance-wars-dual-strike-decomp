#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

/* Task system: does nothing when `task` is NULL. */
void Task_Destroy(void* task);

void func_ov005_02364418(Unk_ov005_0236bbac* work) {
  Task_Destroy(work->unk_50);
}
