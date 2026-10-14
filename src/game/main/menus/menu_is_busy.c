#include "types.h"
#include "game/task.h"

extern TaskScriptCmd data_02159ce0[];
extern TaskScriptCmd data_02159d78[];

void* Task_FindByScript(void* script);
int func_02021b34(void);

BOOL Menu_IsBusy(void) {
  if (Task_FindByScript(data_02159d78) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02159ce0) != NULL) {
    return TRUE;
  }
  return func_02021b34() == 1;
}
