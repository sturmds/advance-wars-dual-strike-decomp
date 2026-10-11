#include "types.h"
#include "game/task.h"
#include "game/touch_marker_task.h"

extern const TaskScriptCmd data_02169494[];

Task *Task_FindByScript(const TaskScriptCmd *script);

/* Starts the touch marker task on a tile unless one exists. */
void TouchMarker_Create(int tileX, int tileY)
{
    TouchMarkerTask *task;

    if (Task_FindByScript(data_02169494) != NULL) {
        return;
    }
    task = (TouchMarkerTask *)Task_Create(data_02169494, TASK_LIST(3));
    task->tileX = tileX;
    task->tileY = tileY;
}
