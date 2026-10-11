#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo OSi_ThreadInfo;

void OSi_RemoveThreadFromList(OSThread *thread)
{
    OSThread *prev = NULL;
    OSThread *cur = OSi_ThreadInfo.list;

    while (cur != NULL && cur != thread) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        OSi_ThreadInfo.list = thread->next;
    } else {
        prev->next = thread->next;
    }
}
