#include "types.h"
#include "game/task.h"

void Battle_InitSway(void);
void Battle_UpdateSway(void);

TaskScriptCmd data_ov002_023604c4[] = {
#include "data/ov002/data_ov002_023604c4.inc.c"
};

void func_ov002_0235323c(void) {
  Task_Create(data_ov002_023604c4, TASK_LIST(1));
}
