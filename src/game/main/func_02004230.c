#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02157284[];
extern const TaskScriptCmd data_02157420[];
int func_02004a14(void);

/* Starts one of two child task scripts under parent, chosen by
 * func_02004a14, and stores arg in the child's unk_64. */
void func_02004230(u16 arg, Task* parent) {
  Task* child;

  if (func_02004a14() == 0) {
    child = Task_CreateChild(data_02157420, parent);
  } else {
    child = Task_CreateChild(data_02157284, parent);
  }
  if (child != NULL) {
    child->unk_64 = arg;
  }
}
