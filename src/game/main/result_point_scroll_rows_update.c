#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void func_020388dc(int x, int y, int arg);
void func_020393dc(int x, int y, int arg);
void func_020394a0(int x, int y);
void Task_ResumeScript(ResultPointTask *task);

/* Task update: moves the score rows and their bar left by up to 120 pixels
 * over task->value frames while the points panel stays at x 8, then clears
 * unk_a4 and resumes the script. */
void ResultPoint_ScrollRowsUpdate(ResultPointTask *task)
{
    int x;

    task->frame++;
    x = 8 - Math_Ease(1, 0, 120, task->frame, task->value);
    func_020388dc(x, 16, task->unk_38);
    func_020394a0(x, 160);
    func_020393dc(8, 160, data_0229c734->tally);
    if (task->frame != task->value) {
        return;
    }
    data_0229c734->unk_a4 = 0;
    task->frame = 0;
    Task_ResumeScript(task);
}
