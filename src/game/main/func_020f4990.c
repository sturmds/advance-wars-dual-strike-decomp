#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;
extern const TaskScriptCmd data_02169a10[];

/* Starts the data_02169a10 task on the second screen and gives its
 * data_021694c0 record the starting values 1, 1 and request. */
void func_020f4990(int request)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_021694c0->previousMode = 1;
    data_021694c0->mode = 1;
    data_021694c0->requestedMode = request;
    Task_Create(data_02169a10, TASK_LIST(3));
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
