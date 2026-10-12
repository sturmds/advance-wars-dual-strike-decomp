#include "types.h"
#include "game/task.h"

extern u8 data_02160110[];
u16 Wireless_GetChildAids(void);
int Wireless_GetSharedData(int a0);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void *a0);
void Link_FlagError(void);
int strcmp(int a0, void *a1);
void Task_ResumeScript(Task *task);
#define WAIT_FRAMES(task) (*(s16 *)&(task)->unk_4c)

void func_0207ee08(Task *task)
{
    int i;
    int found;

    if (!func_0207bd94()) {
        return;
    }
    found = 0;
    for (i = 0; i < 2; i++) {
        if ((Wireless_GetChildAids() | 1) & (1 << i)) {
            if (strcmp(Wireless_GetSharedData((u16)i), data_02160110)) {
                found = 1;
                break;
            }
        }
    }
    if (found == 1) {
        func_0207b4b0(data_02160110);
        WAIT_FRAMES(task)++;
        if (WAIT_FRAMES(task) > 300) {
            Link_FlagError();
        }
    } else {
        WAIT_FRAMES(task) = 0;
        func_0207b48c();
        Task_ResumeScript(task);
    }
}
