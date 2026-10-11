#include "types.h"
#include "game/event_queue.h"
#include "game/task.h"
#include "game/Unk_0229fb30.h"

u8 *func_02078138(int player);
void func_0207b48c(void);
int func_0207bd94(void);
void LinkMenu_SelectEntry(int entry);
void func_0208683c(void);
void func_020868a4(void);
void LinkMenu_DecreaseSetting(void);
void LinkMenu_IncreaseSetting(void);
void func_02086b80(void);
void Task_ResumeScript(Task *task);

/* Task step of the link menu. Reads four 2-byte messages (type, argument)
 * from player 0, who runs the menu: 4 confirms (func_020868a4), 5 cancels
 * (func_0208683c), 6 selects an entry, 7 and 8 raise and lower its
 * setting. Without either, player 0's own console handles the keys. */
void LinkMenu_Update(Task *task)
{
    int confirm = 0;
    int cancel = 0;
    u8 *msg;
    int i;

    if (func_0207bd94() == 1) {
        msg = func_02078138(0);
        for (i = 0; i < 4; i++, msg += 2) {
            switch (msg[0]) {
            case 4:
                confirm = 1;
                break;
            case 5:
                cancel = 1;
                break;
            case 6:
                LinkMenu_SelectEntry(msg[1]);
                break;
            case 7:
                LinkMenu_IncreaseSetting();
                break;
            case 8:
                LinkMenu_DecreaseSetting();
                break;
            }
        }
        EventQueue_Drain();
    }
    if (confirm == 1) {
        func_020868a4();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
        return;
    }
    if (cancel == 1) {
        func_0208683c();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
        return;
    }
    if (data_0229fb30.unk_45a == 0) {
        func_02086b80();
    }
}
