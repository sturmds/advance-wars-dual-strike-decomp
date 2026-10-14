#include "types.h"
#include "game/task.h"

s8 Task_DestroyByScript(const void* descriptor);
extern TaskScriptCmd data_0216050c[];
extern TaskScriptCmd data_021604e4[];

void func_02080708(void) {
  Task_DestroyByScript(data_0216050c);
  Task_DestroyByScript(data_021604e4);
}
