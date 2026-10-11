#include "types.h"
#include "game/task.h"

void Task_ResumeScript(Task *task);
void func_020d7e60(Task *task);

/* Script step: advances the frame counter unk_64 (func_020d7e60 advances it
 * too), loads palette frame (unk_64 >> 2) & 15, and ends the step once that
 * frame wraps to 0. The counter is read as signed to match. */
void PaletteAnimTask_Step(Task *task)
{
    task->unk_64 = (s16)task->unk_64 + 1;
    func_020d7e60(task);
    if (((s16)task->unk_64 >> 2) & 0xf) {
        return;
    }
    Task_ResumeScript(task);
}
