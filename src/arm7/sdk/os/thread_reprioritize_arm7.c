#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;
extern OSThread data_arm7_03807960;    /* where ARM9 checks for its idle thread; no ARM7 code seen creates it */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/*
 * ARM7 copy of OS_SetThreadPriority: moves thread to its place in the list
 * for the new priority. Fails for the idle thread and for threads not in
 * the list.
 */
BOOL Thread_Reprioritize_Arm7(OSThread *thread, u32 priority)
{
    OSThread *cur;
    OSThread *prev;
    int irqState;

    cur = data_arm7_0380790c.list;
    prev = NULL;
    irqState = func_arm7_037fdb80();

    while (cur != NULL && cur != thread) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL || cur == &data_arm7_03807960) {
        func_arm7_037fdb94(irqState);
        return FALSE;
    }
    if (cur->priority != priority) {
        if (prev == NULL) {
            data_arm7_0380790c.list = thread->next;
        } else {
            prev->next = thread->next;
        }
        thread->priority = priority;
        Thread_LinkByPriority_Arm7(thread);
        Thread_SwitchToFirstReady_Arm7();
    }
    func_arm7_037fdb94(irqState);
    return TRUE;
}
