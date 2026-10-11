#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo data_022a9930;

void OSi_RemoveThreadFromList(OSThread *thread)
{
    OSThread *prev = NULL;
    OSThread *cur = data_022a9930.list;

    while (cur != NULL && cur != thread) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        data_022a9930.list = thread->next;
    } else {
        prev->next = thread->next;
    }
}
