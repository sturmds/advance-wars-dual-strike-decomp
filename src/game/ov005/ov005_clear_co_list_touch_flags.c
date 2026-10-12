#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba0.h"

/* Clears the flags that the CO list screen's touch buttons set. */
int Ov005_ClearCoListTouchFlags(Ov005Task* task) {
  Unk_ov005_0236bba0* work = task->work;

  work->unk_04 = 0;
  work->unk_08 = 0;
  work->unk_0c = 0;
  work->unk_10 = 0;
  return 0;
}
