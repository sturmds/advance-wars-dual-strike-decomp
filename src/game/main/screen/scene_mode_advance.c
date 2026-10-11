#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;
extern const TaskScriptCmd data_02169900[];

/* Makes the requested mode (unk_0c, set by func_020f4658) the current one and
 * starts its task under parent. Mode 2 is reset to 4 in all three fields. */
void SceneMode_Advance(Task *parent)
{
    data_021694c0->unk_04 = data_021694c0->unk_08;
    data_021694c0->unk_08 = data_021694c0->unk_0c;
    Task_CreateChild(data_02169900, parent);
    if (data_021694c0->unk_08 == 2) {
        data_021694c0->unk_04 = 4;
        data_021694c0->unk_08 = 4;
        data_021694c0->unk_0c = 4;
    }
}
