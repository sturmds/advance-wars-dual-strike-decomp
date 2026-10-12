#include "types.h"
#include "game/task.h"
#include "game/ov003_unit.h"

void Task_ResumeScript(Task* task);

/* High byte of Ov003State.unk_1d6. */
#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

void func_ov003_02363e60(Task* task) {
  if (STATE_BYTE_1D7 == 0) {
    Task_ResumeScript(task);
  }
}
