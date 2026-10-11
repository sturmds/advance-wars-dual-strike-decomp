#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Unk_021694c0Task *task);
void func_01ff8bc8(int layer, int scrollX, int scrollY);

/* Scrolls layer 3 to the next eased value; resumes the script when the
 * duration is reached. */
void EffectScene_ScrollEaseStep(Unk_021694c0Task *task)
{
    func_01ff8bc8(3, 0, Math_Ease(0, task->easeFrom, task->easeTo, task->easeElapsed++, task->easeDuration));
    if (task->easeElapsed == task->easeDuration) {
        Task_ResumeScript(task);
    }
}
