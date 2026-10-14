#include "types.h"
#include "game/task.h"

int Task_FindByScript(void*);
extern TaskScriptCmd data_0215ab00[];
int Task_JumpToLabel(int, int);

int func_02042d7c(void) {
  int t0 = Task_FindByScript(data_0215ab00);
  return Task_JumpToLabel(t0, 1);
}
