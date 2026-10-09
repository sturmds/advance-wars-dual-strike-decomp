#include "types.h"

typedef struct ShowTask ShowTask;
typedef struct ShowTaskObject ShowTaskObject;

struct ShowTask {
    u8 pad_00[0x30];
    ShowTaskObject *object;
};

struct ShowTaskObject {
    u8 pad_00[1];
    u8 unk_01;
};

void Task_Destroy(ShowTask *task);
void func_020b98bc(void);

void AiShowActionTask_Destroy(ShowTask *task)
{
    ShowTaskObject *object = task->object;
    if (object != NULL) {
        object->unk_01 &= ~6;
    }
    Task_Destroy(task);
    func_020b98bc();
}
