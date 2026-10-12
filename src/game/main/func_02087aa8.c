#include "types.h"
#include "game/event_queue.h"
#include "game/task.h"

u8 *Wireless_GetSharedData(int player);
void func_0207b48c(void);
int func_0207bd94(void);
void func_02087764(int a0);
void func_02087790(void);
void func_020877b8(void);
void Task_ResumeScript(Task *task);

/* Task step of a link screen. Reads four 2-byte messages (type, argument) from player 0:
 * 1 selects the entry in the argument and finishes with func_020877b8, 2 finishes with
 * func_02087790, 3 only selects the entry. */
void func_02087aa8(Task *task)
{
    int confirm = 0;
    int cancel = 0;
    u8 *msg;
    int i;

    if (func_0207bd94() == 1) {
        msg = Wireless_GetSharedData(0);
        for (i = 0; i < 4; msg += 2, i++) {
            switch (msg[0]) {
            case 1:
                confirm = 1;
                func_02087764(msg[1]);
                break;
            case 2:
                cancel = 1;
                break;
            case 3:
                func_02087764(msg[1]);
                break;
            }
        }
        EventQueue_Drain();
    }
    if (confirm == 1) {
        func_020877b8();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
        return;
    }
    if (cancel == 1) {
        func_02087790();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
    }
}
