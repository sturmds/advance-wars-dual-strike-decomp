#include "types.h"
#include "sdk/os.h"

/* Removes one mutex from anywhere in the thread's list. */
void Mutex_UnlinkFromOwner(OSThread *thread, OSMutex *mutex)
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
