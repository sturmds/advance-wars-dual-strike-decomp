#include "types.h"

int Task_FindByScript(void*);
extern u8 data_0215adec[];
int Task_JumpToLabel(int, int);

int func_02046fa0(void) {
  int t0 = Task_FindByScript(data_0215adec);
  return Task_JumpToLabel(t0, 1);
}
