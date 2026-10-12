#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fbe2c(OSThreadQueue *queue);
void func_arm7_037fc528(OSThread *thread, OSMutex *mutex);

/* ARM7 copy of OS_UnlockMutex; main already has that symbol. Drops one level of nesting; the last level frees the mutex and wakes its waiters. */
void func_arm7_037fc5b4(OSMutex *mutex)
{
    int irqState = func_arm7_037fdb80();
    OSThread *current = data_arm7_0380790c.current;

    if (mutex->thread == current) {
        mutex->count--;
        if (mutex->count == 0) {
            func_arm7_037fc528(current, mutex);
            mutex->thread = NULL;
            func_arm7_037fbe2c(&mutex->queue);
        }
    }
    func_arm7_037fdb94(irqState);
}
