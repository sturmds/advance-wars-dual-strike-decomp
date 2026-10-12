#include "types.h"

s8 Task_DestroyByScript(const void* descriptor);
extern u8 data_02158838[];
extern u8 data_02158850[];

void func_02021b80(void) {
  Task_DestroyByScript(data_02158838);
  Task_DestroyByScript(data_02158850);
}
