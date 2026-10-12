#include "types.h"

s8 Task_DestroyByScript(const void* descriptor);
extern u8 data_ov002_023695e0[];
extern u8 data_ov002_023695b8[];

void func_ov002_02356410(void) {
  Task_DestroyByScript(data_ov002_023695e0);
  Task_DestroyByScript(data_ov002_023695b8);
}
