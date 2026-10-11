#include "types.h"
#include "game/task.h"

extern u8 data_0215c810[];

u16 func_0207a1dc(void);
int func_02078138(int a0);
int Link_IsActive(void);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void *a0);
void Link_FlagError(void);
int func_02124534(int a0, void *a1);
void Task_ResumeScript(Task *task);

/* The frame counter shares its slot with Task.unk_4c. */
#define WAIT_FRAMES(task) (*(s16 *)&(task)->unk_4c)

void LinkTask_WaitForPeerData(Task *task)
{
    int i;
    int found;

    if (!Link_IsActive()) {
        Task_ResumeScript(task);
        return;
    }
    if (!func_0207bd94()) {
        return;
    }
    found = 0;
    for (i = 0; i < 4; i++) {
        if ((func_0207a1dc() | 1) & (1 << i)) {
            if (func_02124534(func_02078138((u16)i), data_0215c810)) {
                found = 1;
                break;
            }
        }
    }
    if (found == 1) {
        func_0207b4b0(data_0215c810);
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
