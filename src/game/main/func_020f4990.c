#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;
extern u8 data_02169a10[];

void *Task_Create(const void *script, void *parent);

/* Starts the data_02169a10 task on the second screen and gives its
 * data_021694c0 record the starting values 1, 1 and request. */
void func_020f4990(int request)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_021694c0->unk_04 = 1;
    data_021694c0->unk_08 = 1;
    data_021694c0->unk_0c = request;
    Task_Create(data_02169a10, (void *)3);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
