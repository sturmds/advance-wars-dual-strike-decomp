#include "types.h"
#include "game/task.h"

extern s16 data_021584c8[][2];
extern s16 data_021584ca[][2];

#define CURSOR(task) (*(s16 *)&(task)->waitFrames)

/* Redraws the cursor of the event choice prompt: clears the two-tile column
 * of every choice marker in the task's tile map, then puts tiles 0x1b1 and
 * 0x1b2 (palette from unk_40) at the marker of the current choice and calls
 * unk_3c. */
void func_0201a4f0(Task *task)
{
    u16 (*tiles)[32] = *(u16 (**)[32])&task->unk_34;
    int i;

    for (i = 0; i < 4; i++) {
        tiles[data_021584c8[i][1] * 32][data_021584c8[i][0]] = 0;
        tiles[data_021584c8[i][1] * 32][data_021584c8[i][0] + 32] = 0;
    }
    tiles[data_021584ca[CURSOR(task)][0]][data_021584c8[CURSOR(task)][0]] = task->unk_40 | 0x1b1;
    tiles[data_021584ca[CURSOR(task)][0]][data_021584c8[CURSOR(task)][0] + 32] = task->unk_40 | 0x1b2;
    task->unk_3c();
}
