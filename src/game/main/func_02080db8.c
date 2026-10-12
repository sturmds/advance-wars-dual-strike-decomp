#include "types.h"
#include "game/task.h"

extern u8 data_021604dc[];

u16 Wireless_GetChildAids(void);
int Wireless_GetSharedData(int a0);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void *a0);
void Link_FlagError(void);
int strcmp(int a0, void *a1);
void Task_ResumeScript(Task *task);

void func_02080db8(Task *task)
{
    int i;
    int found;

    if (!func_0207bd94()) {
        return;
    }
    found = 0;
    for (i = 0; i < 4; i++) {
        if ((Wireless_GetChildAids() | 1) & (1 << i)) {
            if (strcmp(Wireless_GetSharedData((u16)i), data_021604dc)) {
                found = 1;
                break;
            }
        }
    }
    if (found == 1) {
        func_0207b4b0(data_021604dc);
        task->waitFrames++;
        if (task->waitFrames > 300) {
            Link_FlagError();
        }
    } else {
        task->waitFrames = 0;
        func_0207b48c();
        Task_ResumeScript(task);
    }
}
