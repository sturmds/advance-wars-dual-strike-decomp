#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo OSi_ThreadInfo;

void OSi_InsertThreadToList(OSThread *thread)
{
    OSThread *cur;
    OSThread *prev;
    OSThread *head;

    prev = NULL;
    head = OSi_ThreadInfo.list;
    cur = head;
    while (cur != NULL && cur->priority < thread->priority) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        thread->next = head;
        OSi_ThreadInfo.list = thread;
    } else {
        thread->next = prev->next;
        prev->next = thread;
    }
}
