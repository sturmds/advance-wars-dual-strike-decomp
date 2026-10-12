#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

void func_arm7_037fc18c(OSThread *thread)
{
    OSThread *cur = data_arm7_0380790c.list;
    OSThread *prev = NULL;

    while (cur != NULL && cur != thread) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        data_arm7_0380790c.list = thread->next;
    } else {
        prev->next = thread->next;
    }
}
