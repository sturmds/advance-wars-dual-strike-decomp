#include "types.h"
#include "game/Unk_ov005_0235a30c.h"

/* Task system: does nothing when `task` is NULL. */
void Task_Destroy(void* task);

void func_ov005_0235a30c(Unk_ov005_0235a30c* work) {
  Task_Destroy(work->unk_8348);
}
