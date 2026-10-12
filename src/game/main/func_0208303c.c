#include "types.h"

int func_0207bd94(void);
void Task_ResumeScript(void* task);

void func_0208303c(void* task) {
  if (func_0207bd94() != 1) {
    return;
  }
  Task_ResumeScript(task);
}
