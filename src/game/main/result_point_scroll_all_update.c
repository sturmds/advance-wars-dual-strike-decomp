#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void ResultPoint_DrawRows(int x, int y, int arg);
void ResultPoint_DrawPanel(int x, int y, int arg);
void ResultPoint_DrawBar(int x, int y);
void Task_ResumeScript(ResultPointTask *task);

/* Task update: moves the score rows, their bar and the points panel left by
 * up to 120 pixels over task->value frames, then clears unk_a4 and resumes
 * the script. */
void ResultPoint_ScrollAllUpdate(ResultPointTask *task)
{
    int x;

    task->frame++;
    x = 8 - Math_Ease(1, 0, 120, task->frame, task->value);
    ResultPoint_DrawRows(x, 16, task->countStep);
    ResultPoint_DrawBar(x, 160);
    ResultPoint_DrawPanel(x, 160, data_0229c734->tally);
    if (task->frame != task->value) {
        return;
    }
    data_0229c734->unk_a4 = 0;
    task->frame = 0;
    Task_ResumeScript(task);
}
