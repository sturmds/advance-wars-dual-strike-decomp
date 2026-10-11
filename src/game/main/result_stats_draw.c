#include "types.h"
#include "game/task.h"

void ResultStats_DrawSubScreen(int offset, Task *task);
void func_0203b878(int offset, Task *task);

/* Draws both screens of the stats page, moved right by `offset` pixels. */
void ResultStats_Draw(int offset, Task *task)
{
    ResultStats_DrawSubScreen(offset, task);
    func_0203b878(offset, task);
}
