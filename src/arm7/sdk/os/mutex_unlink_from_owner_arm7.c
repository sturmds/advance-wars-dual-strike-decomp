#include "types.h"
#include "sdk/os_arm7.h"

/* ARM7 copy of Mutex_UnlinkFromOwner; main already has that symbol. Removes one mutex from anywhere in the thread's list. */
void func_arm7_037fc528(OSThread *thread, OSMutex *mutex)
{
    OSMutex *next = mutex->next;
    OSMutex *prev = mutex->prev;

    if (next == NULL) {
        thread->mutexQueue.tail = prev;
    } else {
        next->prev = prev;
    }
    if (prev == NULL) {
        thread->mutexQueue.head = next;
    } else {
        prev->next = next;
    }
}
