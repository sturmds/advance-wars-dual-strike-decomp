#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb80.h"

void Ov005_SetHasTaskB(Ov005Task* task) {
  ((Unk_ov005_0236bb80*)task->work)->hasTaskB = 1;
}
