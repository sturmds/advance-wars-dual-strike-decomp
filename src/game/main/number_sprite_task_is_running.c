#include "types.h"
#include "game/task.h"

void* Task_FindByScript(const TaskScriptCmd* script);
extern const TaskScriptCmd gNumberSpriteTaskScript[];

BOOL NumberSpriteTask_IsRunning(void) {
  return Task_FindByScript(gNumberSpriteTaskScript) != NULL;
}
