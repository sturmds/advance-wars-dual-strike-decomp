#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd gTouchMarkerScript[];

Task *Task_FindByScript(const TaskScriptCmd *script);

BOOL TouchMarker_Exists(void)
{
    return Task_FindByScript(gTouchMarkerScript) != NULL;
}
