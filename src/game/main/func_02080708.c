#include "types.h"

s8 Task_DestroyByScript(const void* descriptor);
extern u8 data_0216050c[];
extern u8 data_021604e4[];

void func_02080708(void) {
  Task_DestroyByScript(data_0216050c);
  Task_DestroyByScript(data_021604e4);
}
