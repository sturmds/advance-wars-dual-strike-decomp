#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(void* script);
extern u8 data_02158838[];
extern u8 data_02158850[];

/* TRUE while a task started with either script exists. */
BOOL func_02021b34(void) {
  if (Task_FindByScript(data_02158838) != NULL) {
    return TRUE;
  }
  return Task_FindByScript(data_02158850) != NULL;
}
