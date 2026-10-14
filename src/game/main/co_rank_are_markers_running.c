#include "types.h"
#include "game/task.h"

extern TaskScriptCmd data_0215af94[];
extern TaskScriptCmd data_0215b004[];
void* Task_FindByScript(void* script);
int func_02021b34(void);

BOOL CoRank_AreMarkersRunning(void) {
  if (Task_FindByScript(data_0215b004) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_0215af94) != NULL) {
    return TRUE;
  }
  return func_02021b34() == 1;
}
