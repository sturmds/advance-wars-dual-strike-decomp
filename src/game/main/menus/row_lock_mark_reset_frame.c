#include "types.h"
#include "game/row_lock_mark_task.h"

/* Step of all four row lock mark scripts: restarts the frame count. */
void RowLockMark_ResetFrame(RowLockMarkTask *task)
{
    task->frame = 0;
}
