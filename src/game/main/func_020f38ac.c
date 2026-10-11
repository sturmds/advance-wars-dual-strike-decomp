#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern const TaskScriptCmd data_021668e8[];

void func_020f35e0(int value);
int func_020f4658(int value);

/* Sets scene fields 0x00 to 0 and 0x0c to 5 (through func_020f35e0 and
 * func_020f4658). On the first screen it also starts the data_021668e8 child
 * task of parent. */
void func_020f38ac(Task *parent)
{
    if (data_027c027c == data_027c0288.unk_00) {
        func_020f35e0(0);
        func_020f4658(5);
        Task_CreateChild(data_021668e8, parent);
    } else {
        func_020f35e0(0);
        func_020f4658(5);
    }
}
