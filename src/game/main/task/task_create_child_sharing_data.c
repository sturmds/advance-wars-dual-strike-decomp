#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215b1bc[];

void Task_CreateChildSharingData(Task* parent) {
  Task* child = Task_Create(data_0215b1bc, parent);
  child->unk_4c = parent->unk_4c;
  child->unk_38 = parent->unk_38;
}
