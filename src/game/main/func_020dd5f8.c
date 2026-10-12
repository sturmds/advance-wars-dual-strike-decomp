#include "types.h"

BOOL func_02019bec(void);
void Task_ResumeScript(void* task);

void func_020dd5f8(void* task) {
  if (func_02019bec() != FALSE) {
    return;
  }
  Task_ResumeScript(task);
}
