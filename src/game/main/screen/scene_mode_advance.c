#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;
extern const TaskScriptCmd data_02169900[];

/* Makes the requested mode (set by SceneMode_Request) the current one and
 * starts its task under parent. Mode 2 is reset to 4 in all three fields. */
void SceneMode_Advance(Task *parent)
{
    data_021694c0->previousMode = data_021694c0->mode;
    data_021694c0->mode = data_021694c0->requestedMode;
    Task_CreateChild(data_02169900, parent);
    if (data_021694c0->mode == 2) {
        data_021694c0->previousMode = 4;
        data_021694c0->mode = 4;
        data_021694c0->requestedMode = 4;
    }
}
