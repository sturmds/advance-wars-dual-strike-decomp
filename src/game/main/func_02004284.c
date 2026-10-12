#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_021572c4[];
extern const TaskScriptCmd data_021573c0[];
int func_02004a14(void);

void func_02004284(u16 arg, Task* parent) {
  Task* child;

  if (func_02004a14() == 0) {
    child = Task_CreateChild(data_021573c0, parent);
  } else {
    child = Task_CreateChild(data_021572c4, parent);
  }
  if (child != NULL) {
    child->unk_64 = arg;
  }
}
