#include "types.h"
#include "sdk/os.h"

/* Removes the oldest mutex from the thread's list and returns it. */
OSMutex *Mutex_PopOldestHeld(OSThread *thread)
{
    OSMutex *head = thread->mutexQueue.head;
    OSMutex *next = head->next;

    if (next == NULL) {
        thread->mutexQueue.tail = NULL;
    } else {
        next->prev = NULL;
    }
    thread->mutexQueue.head = next;
    return head;
}
