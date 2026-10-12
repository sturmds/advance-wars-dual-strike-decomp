#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern const TaskScriptCmd data_02169878[];

/* Starts the data_02169878 task, then sets four starting values in the
 * data_021694c0 record: 0x80 and 60 in fields 0x24 and 0x28, 0 in 0x2c and
 * 0x30. */
void func_020f28d4(void* parent) {
  Task_Create(data_02169878, parent);
  data_021694c0->unk_24 = 0x80;
  data_021694c0->unk_28 = 60;
  data_021694c0->unk_2c = 0;
  data_021694c0->unk_30 = 0;
}
