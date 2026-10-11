#include "types.h"
#include "game/event_queue.h"
#include "game/keypad.h"
#include "game/menu_state.h"
#include "game/task.h"

u16 func_0207a1dc(void);
u8 *func_02078138(int player);
void func_0207b48c(void);
int func_0207bd6c(void);
int func_0207bd80(void);
int func_0207bd94(void);
int Link_FindSlotOfPlayer(int player);
u32 func_02073874(void);
void func_020667bc(int msg); /* previous team for slot msg - 19 */
void func_02066760(int msg); /* next team for slot msg - 23 */
void func_020666cc(int slot); /* sets slot ready */
void func_02066674(int slot); /* clears slot ready */
void *func_02066624(void);
void *func_0206664c(void);
void Task_ResumeScript(Task *task);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Link task step of the Vs setup team screen. Applies the team messages
 * of every connected machine (setup.unk_2d0 holds each slot's ready
 * flag). Back from a slot that is not ready leaves the screen; once every
 * linked slot is ready the screen goes on. Both clear the ready flags and
 * resume the task's script. A ready player can press B to send back. */
void VsSetup_UpdateTeamScreenLink(Task *task)
{
    int allReady;
    u8 *msg;
    int cancel;
    int j;
    int i;

    allReady = 1;
    cancel = 0;
    if (func_0207bd80() != 1) {
        return;
    }
    if (func_0207bd94() == 1) {
        for (i = 0; i < 4; i++) {
            if ((func_0207a1dc() | 1) & (1 << i)) {
                msg = func_02078138((u16)i);
                for (j = 0; j < 4; j++, msg += 2) {
                    switch (msg[0]) {
                    case EVENT_VS_TEAM_PREV + 0:
                    case EVENT_VS_TEAM_PREV + 1:
                    case EVENT_VS_TEAM_PREV + 2:
                    case EVENT_VS_TEAM_PREV + 3:
                        func_020667bc(msg[0]);
                        break;
                    case EVENT_VS_TEAM_NEXT + 0:
                    case EVENT_VS_TEAM_NEXT + 1:
                    case EVENT_VS_TEAM_NEXT + 2:
                    case EVENT_VS_TEAM_NEXT + 3:
                        func_02066760(msg[0]);
                        break;
                    case EVENT_VS_TEAM_READY:
                        func_020666cc((u8)(Link_FindSlotOfPlayer(i) - 1));
                        break;
                    case EVENT_VS_TEAM_BACK:
                        if (gMenuState.setup.unk_2d0[Link_FindSlotOfPlayer(i) - 1] == 0) {
                            cancel = 1;
                        } else {
                            func_02066674((u8)(Link_FindSlotOfPlayer(i) - 1));
                        }
                        break;
                    }
                }
            }
        }
        EventQueue_Drain();
    }
    if (cancel == 1) {
        func_0206664c();
        func_0207b48c();
        EventQueue_Reset();
        for (i = 0; i < 4; i++) {
            gMenuState.setup.unk_2d0[i] = 0;
        }
        Task_ResumeScript(task);
        return;
    }
    for (i = 0; i < 4; i++) {
        if (func_02073874() & (1 << i)) {
            if (gMenuState.setup.unk_2d0[i] == 0) {
                allReady = 0;
                break;
            }
        }
    }
    if (allReady == 1) {
        func_02066624();
        func_0207b48c();
        EventQueue_Reset();
        for (i = 0; i < 4; i++) {
            gMenuState.setup.unk_2d0[i] = 0;
        }
        Task_ResumeScript(task);
        return;
    }
    if (MENU->setup.unk_2d0[Link_FindSlotOfPlayer(func_0207bd6c()) - 1] == 0) {
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        EventQueue_Push(EVENT_VS_TEAM_BACK, 0);
    }
}
