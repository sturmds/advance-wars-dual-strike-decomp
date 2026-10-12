#include "types.h"

s8 Task_DestroyByScript(const void* descriptor);
extern u8 data_02159134[];
extern u8 data_02159114[];

void func_02027a6c(void) {
  Task_DestroyByScript(data_02159134);
  Task_DestroyByScript(data_02159114);
}
