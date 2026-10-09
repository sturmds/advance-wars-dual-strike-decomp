#include "types.h"

typedef struct Unk_02004a1c_0 {
    u8 pad_00[0x64];
    u16 unk_64;
} Unk_02004a1c_0;
Unk_02004a1c_0 *Task_CreateChild(void *script);
void func_02004bac(void);
extern u8 data_02157324[];

void func_02004a4c(int value)
{
    Unk_02004a1c_0 *task = Task_CreateChild(data_02157324);

    if (task != NULL) {
        task->unk_64 = value;
        func_02004bac();
    }
}
