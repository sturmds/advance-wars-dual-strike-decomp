#include "types.h"
#include "game/Unk_02080440.h"
#include "game/missile_strike_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void Task_ResumeScript(MissileStrikeTask *task);
void func_01ff8bc8(int layer, int scrollX, int scrollY);

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Update of data_02169088's fall step: layer 0 (the missile picture) moves
 * down 16 pixels a frame toward the landing pixel; unk_54 is the height left.
 * At 0 the script resumes. */
void MissileStrike_FallStep(MissileStrikeTask *task)
{
    int x = task->pixelX - VIEW->scrollX;
    int y = task->pixelY - VIEW->scrollY;

    task->unk_54 -= 16;
    if (task->unk_54 < 0) {
        task->unk_54 = 0;
        Task_ResumeScript(task);
    }
    func_01ff8bc8(0, 40 - x, 192 - y + task->unk_54);
}
