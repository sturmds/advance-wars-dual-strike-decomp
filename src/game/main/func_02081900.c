#include "types.h"
#include "game/task.h"

extern u8 data_0216061c[];

u16 Wireless_GetChildAids(void);
int func_02078138(int a0);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void *a0);
void Link_FlagError(void);
int func_02124534(int a0, void *a1);
void Task_ResumeScript(Task *task);

void func_02081900(Task *task)
{
    int i;
    int found;

    if (!func_0207bd94()) {
        return;
    }
    found = 0;
    for (i = 0; i < 4; i++) {
        if ((Wireless_GetChildAids() | 1) & (1 << i)) {
            if (func_02124534(func_02078138((u16)i), data_0216061c)) {
                found = 1;
                break;
            }
        }
    }
    if (found == 1) {
        func_0207b4b0(data_0216061c);
        task->unk_34++;
        if (task->unk_34 > 300) {
            Link_FlagError();
        }
    } else {
        task->unk_34 = 0;
        func_0207b48c();
        Task_ResumeScript(task);
    }
}
