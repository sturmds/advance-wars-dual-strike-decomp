#include "types.h"
#include "game/Unk_0216928c.h"

Unk_0216928c *Task_Create(const void *script, void *parentOrList);
Unk_0216928c *Task_CreateChild(const void *script, void *parent);

extern u8 data_0216928c[];

/* Starts the data_0216928c task, as a child of `parent` when it is not NULL,
 * and stores a, b and c in it. */
void func_020eee1c(int a, int b, int c, void *parent)
{
    Unk_0216928c *task;

    if (parent != NULL) {
        task = Task_CreateChild(data_0216928c, parent);
    } else {
        task = Task_Create(data_0216928c, (void *)3);
    }
    task->unk_2c = a;
    task->unk_30 = b;
    task->unk_4a = c;
    task->unk_54 = 1;
}
