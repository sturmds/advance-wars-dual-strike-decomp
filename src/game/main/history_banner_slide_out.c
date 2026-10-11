#include "types.h"
#include "game/task.h"

#define FRAME(task) (*(s16 *)&(task)->unk_4c)

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task *task);
void func_02070e30(int x, int y);

/* Script task: slides the banner left from x 128 to -64 over 16 frames, then
 * resumes the script. */
void HistoryBanner_SlideOut(Task *task)
{
    FRAME(task)++;
    func_02070e30(Math_Ease(1, 0, -192, FRAME(task), 16) + 128, 96);
    if (FRAME(task) != 16) {
        return;
    }
    FRAME(task) = 0;
    Task_ResumeScript(task);
}
