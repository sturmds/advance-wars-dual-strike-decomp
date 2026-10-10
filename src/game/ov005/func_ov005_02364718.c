#include "types.h"
#include "game/task.h"
#include "game/Unk_ov005_0236bbac.h"

extern const TaskScriptCmd data_ov005_0236aedc[];

void func_ov005_02364718(Unk_ov005_0236bbac *state)
{
    state->pictureX = 200 << 12;
    state->pictureY = 160 << 12;
    state->unk_48 = Task_Create(data_ov005_0236aedc, TASK_LIST(3));
}
