#include "types.h"
#include "game/task.h"

BOOL Sound_IsSongFadingOut(void);

void func_0201b130(Task* task) {
  if (Sound_IsSongFadingOut() == FALSE) {
    task->pc = NULL;
  }
}
