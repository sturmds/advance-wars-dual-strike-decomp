#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(void* script);
extern TaskScriptCmd data_0215b470[];
extern TaskScriptCmd data_0215b4a0[];

BOOL func_0204ec78(void) {
  if (Task_FindByScript(data_0215b470) != NULL) {
    return TRUE;
  }
  return Task_FindByScript(data_0215b4a0) != NULL;
}
