#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd gTouchMarkerScript[];

Task *Task_FindByScript(const TaskScriptCmd *script);
void Task_DestroyByScript(const TaskScriptCmd *script);

/* Destroys the touch marker task, if there is one. */
void TouchMarker_Destroy(void)
{
    if (Task_FindByScript(gTouchMarkerScript) == NULL) {
        return;
    }
    Task_DestroyByScript(gTouchMarkerScript);
}
