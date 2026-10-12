#include "types.h"

s8 Task_DestroyByScript(const void* descriptor);
extern u8 data_ov003_02364aac[];
extern u8 data_ov003_02364acc[];

void func_ov003_0235ca24(void) {
  Task_DestroyByScript(data_ov003_02364aac);
  Task_DestroyByScript(data_ov003_02364acc);
}
