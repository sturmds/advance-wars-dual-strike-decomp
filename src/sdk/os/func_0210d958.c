#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo data_022a9930;

void func_0210d958(OSThread *thread)
{
    OSThread *cur;
    OSThread *prev;
    OSThread *head;

    prev = NULL;
    head = data_022a9930.list;
    cur = head;
    while (cur != NULL && cur->priority < thread->priority) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        thread->next = head;
        data_022a9930.list = thread;
    } else {
        thread->next = prev->next;
        prev->next = thread;
    }
}
