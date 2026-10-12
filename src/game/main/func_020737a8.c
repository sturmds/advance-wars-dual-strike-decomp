#include "types.h"
#include "game/task.h"
#include "sdk/msl_string.h"

extern char data_0215f810[];
u16 Wireless_GetChildAids(void);
u16 *Wireless_GetSharedData(u16 aid);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void *a0);
void Link_FlagError(void);
void Task_ResumeScript(Task *task);
#define WAIT_FRAMES(task) (*(s16 *)&(task)->unk_4c)

void func_020737a8(Task *task)
{
    int i;
    int found;

    if (!func_0207bd94()) {
        return;
    }
    found = 0;
    for (i = 0; i < 4; i++) {
        if ((Wireless_GetChildAids() | 1) & (1 << i)) {
            if (strcmp((const char *)Wireless_GetSharedData((u16)i), data_0215f810)) {
                found = 1;
                break;
            }
        }
    }
    if (found == 1) {
        func_0207b4b0(data_0215f810);
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
