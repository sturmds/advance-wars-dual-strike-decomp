#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

void ResultPoint_DrawRows(int x, int y, int arg);
void ResultPoint_DrawPanel(int x, int y, int arg);
void ResultPoint_DrawBar(int x, int y);

/* Task update: draws the score rows, their bar and the points panel at
 * x 8. */
void ResultPoint_DrawUpdate(ResultPointTask *task)
{
    ResultPoint_DrawRows(8, 16, task->countStep);
    ResultPoint_DrawBar(8, 160);
    ResultPoint_DrawPanel(8, 160, data_0229c734->tally);
}
