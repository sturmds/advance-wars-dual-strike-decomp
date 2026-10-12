#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb80.h"

void Ov005_ClearHasTaskB(Ov005Task* task) {
  Unk_ov005_0236bb80* work = task->work;

  work->hasTaskB = 0;
}
