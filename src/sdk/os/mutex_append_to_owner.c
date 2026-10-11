#include "types.h"
#include "sdk/os.h"

/* Adds a mutex to the end of the thread's list. */
void Mutex_AppendToOwner(OSThread *thread, OSMutex *mutex)
{
    OSMutex *tail = thread->mutexQueue.tail;

    if (tail == NULL) {
        thread->mutexQueue.head = mutex;
    } else {
        /* The original links the old head here, not the old tail. */
        thread->mutexQueue.head->next = mutex;
    }
    mutex->prev = tail;
    mutex->next = NULL;
    thread->mutexQueue.tail = mutex;
}
