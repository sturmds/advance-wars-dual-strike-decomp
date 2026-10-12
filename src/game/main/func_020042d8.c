#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02157304[];
extern const TaskScriptCmd data_021573f0[];
int func_02004a14(void);

void func_020042d8(u16 arg, Task* parent) {
  Task* child;

  if (func_02004a14() == 0) {
    child = Task_CreateChild(data_021573f0, parent);
  } else {
    child = Task_CreateChild(data_02157304, parent);
  }
  if (child != NULL) {
    child->unk_64 = arg;
  }
}
