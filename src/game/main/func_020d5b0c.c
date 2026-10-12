#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(void* script);
extern u8 data_02166ed4[];
extern u8 data_02166efc[];

BOOL func_020d5b0c(void) {
  if (Task_FindByScript(data_02166ed4) != NULL) {
    return TRUE;
  }
  return Task_FindByScript(data_02166efc) != NULL;
}
