#include "types.h"
#include "game/task.h"

/* Script whose main step is Ov005_UpdateCoScreen. */
extern const TaskScriptCmd data_ov005_0236abfc[];

void func_ov005_02361544(void* parent) {
  Task_CreateChild(data_ov005_0236abfc, parent);
}
