#include "types.h"

extern u32 data_0229eaec;
extern u8 data_021604e4[];
void *Task_Create(const void *script, void *parentOrList);

/* Starts the data_021604e4 task (func_02080708 destroys it) while data_0229eaec is set. */
void func_02080734(void)
{
    if (data_0229eaec == 0) {
        return;
    }
    Task_Create(data_021604e4, (void *)3);
}
