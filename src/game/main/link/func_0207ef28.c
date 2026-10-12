#include "types.h"
#include "game/task.h"

int func_0207a1ac(void);
void func_0207b564(void);

/* Task step. Does nothing while func_0207a1ac is nonzero. Then it runs the
 * link state 0 to 1 step (func_0207b564) and clears the frame counter. */
void func_0207ef28(Task* task) {
  if (func_0207a1ac() != 0) {
    return;
  }
  func_0207b564();
  task->waitFrames = 0;
}
