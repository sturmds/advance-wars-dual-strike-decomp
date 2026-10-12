#include "types.h"
#include "game/task.h"

void Task_DestroyByScript(const TaskScriptCmd* script);

extern TaskScriptCmd gScreenShakeScript[];

void ScreenShake_Stop(void) { Task_DestroyByScript(gScreenShakeScript); }
