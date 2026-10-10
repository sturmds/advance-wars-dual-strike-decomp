#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbb0.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Ov005Task *task);
void func_0201cbbc(u32 level);

extern Unk_ov005_0236bbb0 *data_ov005_0236bbb0;

/* Eases a level over 8 frames, from 6 down to 0 when unk_14 is 1 and from 0
 * up to 6 otherwise, and passes it to func_0201cbbc each frame (meaning
 * unknown). Step 2 clears unk_14 and resumes the task script. */
void Ov005_UpdateShopTransition(Ov005Task *task)
{
    Unk_ov005_0236bbb0 *work = data_ov005_0236bbb0;
    int level;

    switch (task->step) {
    case 0:
        task->frame = 0;
        task->frames = 8;
        task->step++;
    case 1:
        if (work->unk_14 == 1) {
            level = Math_Ease(0, 0x6000, 0, task->frame, task->frames);
        } else {
            level = Math_Ease(0, 0, 0x6000, task->frame, task->frames);
        }
        if (task->frame >= task->frames) {
            task->step++;
        } else {
            task->frame++;
        }
        func_0201cbbc((level >> 12) & 0xff);
        return;
    case 2:
        work->unk_14 = 0;
        Task_ResumeScript(task);
        return;
    }
}
