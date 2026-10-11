#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void func_020388dc(int x, int y, int arg);
void func_020393dc(int x, int y, int arg);
void func_020394a0(int x, int y);
void Task_ResumeScript(ResultPointTask *task);

#define SLIDE_IN_FRAMES 24

/* Task update: slides the score rows, their bar and the points panel in
 * from x -160 to x 8 over SLIDE_IN_FRAMES frames, then resumes the script. */
void ResultPoint_SlideInUpdate(ResultPointTask *task)
{
    int x = Math_Ease(4, -160, 8, task->frame, SLIDE_IN_FRAMES);

    func_020388dc(x, 16, -1);
    func_020394a0(x, 160);
    func_020393dc(x, 160, -1);
    if (task->frame != SLIDE_IN_FRAMES) {
        task->frame++;
        return;
    }
    task->frame = 0;
    Task_ResumeScript(task);
}
