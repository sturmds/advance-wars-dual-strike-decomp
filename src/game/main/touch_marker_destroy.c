#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02169494[];

Task *Task_FindByScript(const TaskScriptCmd *script);
void Task_DestroyByScript(const TaskScriptCmd *script);

/* Destroys the touch marker task, if there is one. */
void TouchMarker_Destroy(void)
{
    if (Task_FindByScript(data_02169494) == NULL) {
        return;
    }
    Task_DestroyByScript(data_02169494);
}
