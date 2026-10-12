#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_021572a4[];
extern const TaskScriptCmd data_02157390[];
int func_02004a14(void);

void func_0200432c(u16 arg, Task* parent) {
  Task* child;

  if (func_02004a14() == 0) {
    child = Task_CreateChild(data_02157390, parent);
  } else {
    child = Task_CreateChild(data_021572a4, parent);
  }
  if (child != NULL) {
    child->unk_64 = arg;
  }
}
