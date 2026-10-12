#include "types.h"
#include "sdk/os_arm7.h"

/* ARM7 copy of Mutex_AppendToOwner; main already has that symbol. Adds a mutex to the end of the thread's list. */
void func_arm7_037fc54c(OSThread *thread, OSMutex *mutex)
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
