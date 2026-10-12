#include "types.h"
#include "game/task.h"

BOOL func_02038164(void);
void Task_ResumeScript(Task* task);
void func_0208b95c(void);

void func_0208b1dc(Task* task) {
  if (func_02038164() != FALSE) {
    return;
  }
  func_0208b95c();
  Task_ResumeScript(task);
}
