#include "types.h"
#include "game/task.h"
#include "game/link_marker_task.h"

extern const TaskScriptCmd data_021601d0[];
extern const TaskScriptCmd data_02160208[];
extern const TaskScriptCmd data_02160240[];

/* Starts a marker task next to a button: kind 0 for the stage 1 pick, player + 1 for the stage 2 pick of a player. */
void LinkMenu_CreateMarker(u8 kind, u16 x, u16 y)
{
    LinkMarkerTask *task;

    switch (kind) {
    case 0:
        task = (LinkMarkerTask *)Task_Create(data_021601d0, TASK_LIST(3));
        break;
    case 1:
        task = (LinkMarkerTask *)Task_Create(data_02160208, TASK_LIST(3));
        break;
    default:
        task = (LinkMarkerTask *)Task_Create(data_02160240, TASK_LIST(3));
        break;
    }
    task->x = x;
    task->y = y;
    task->affineIndex = kind + 25;
}
