#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

void func_020388dc(int x, int y, int arg);
void func_020393dc(int x, int y, int arg);
void func_020394a0(int x, int y);

/* Task update: draws the score rows, their bar and the points panel at
 * x 8. */
void ResultPoint_DrawUpdate(ResultPointTask *task)
{
    func_020388dc(8, 16, task->unk_38);
    func_020394a0(8, 160);
    func_020393dc(8, 160, data_0229c734->tally);
}
