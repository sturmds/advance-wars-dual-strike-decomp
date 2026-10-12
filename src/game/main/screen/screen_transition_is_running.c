#include "types.h"
#include "game/task.h"

void* Task_FindByScript(const TaskScriptCmd* script);

extern const TaskScriptCmd data_02157224[];
extern const TaskScriptCmd data_02157244[];
extern const TaskScriptCmd data_02157264[];
extern const TaskScriptCmd data_02157284[];
extern const TaskScriptCmd data_021572a4[];
extern const TaskScriptCmd data_021572c4[];
extern const TaskScriptCmd data_021572e4[];
extern const TaskScriptCmd data_02157304[];
extern const TaskScriptCmd data_02157324[];
extern const TaskScriptCmd data_02157344[];
extern const TaskScriptCmd data_02157390[];
extern const TaskScriptCmd data_021573c0[];
extern const TaskScriptCmd data_021573f0[];
extern const TaskScriptCmd data_02157420[];

/* TRUE while any screen fade, blend or wipe task is running. */
BOOL ScreenTransition_IsRunning(void) {
  if (Task_FindByScript(data_02157264) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157244) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021573f0) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157390) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157420) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021573c0) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157304) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021572a4) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157284) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021572c4) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157344) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_021572e4) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02157224) != NULL) {
    return TRUE;
  }
  return Task_FindByScript(data_02157324) != NULL;
}
