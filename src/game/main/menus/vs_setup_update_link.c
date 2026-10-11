#include "types.h"
#include "game/event_queue.h"
#include "game/keypad.h"
#include "game/menu_state.h"
#include "game/task.h"

u16 func_0207a1dc(void);
u8 *func_02078138(int player);
void func_0207b48c(void);
int Link_GetOwnPlayer(void);
int Link_IsActive(void);
int func_0207bd94(void);
int Link_FindSlotOfPlayer(int player);
u32 Link_GetConnectedSlotMask(void);
void VsSetup_SetController(int event, int controller);
void VsSetup_SetCo(int event, int co);
void VsSetup_Cancel(void);
void VsSetup_SetPlayerReady(int slot);
void VsSetup_ClearPlayerReady(int player);
void func_02067b0c(void);
void Task_ResumeScript(Task *task);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Link task step of the Vs setup screen, like VsSetup_UpdateTeamScreenLink.
 * Applies the controller, CO and ready messages of every connected machine.
 * Cancel from a slot that is not ready leaves the screen; from a ready slot
 * it clears that slot's ready flag. Once every linked slot is ready the
 * screen goes on. Leaving or going on clears the ready flags and resumes the
 * task's script. A ready player can press B to send a cancel. */
void VsSetup_UpdateLink(Task *task)
{
    int allReady;
    u8 *msg;
    int cancel;
    int j;
    int i;

    cancel = 0;
    MENU->setup.cursorSePlayed = 0;
    allReady = 1;
    if (Link_IsActive() != 1) {
        return;
    }
    if (func_0207bd94() == 1) {
        for (i = 0; i < 4; i++) {
            if ((func_0207a1dc() | 1) & (1 << i)) {
                msg = func_02078138((u16)i);
                for (j = 0; j < 4; j++, msg += 2) {
                    switch (msg[0]) {
                    case EVENT_VS_SETUP_CONTROLLER + 0:
                    case EVENT_VS_SETUP_CONTROLLER + 1:
                    case EVENT_VS_SETUP_CONTROLLER + 2:
                    case EVENT_VS_SETUP_CONTROLLER + 3:
                    case EVENT_VS_SETUP_CONTROLLER + 4:
                    case EVENT_VS_SETUP_CONTROLLER + 5:
                    case EVENT_VS_SETUP_CONTROLLER + 6:
                    case EVENT_VS_SETUP_CONTROLLER + 7:
                        VsSetup_SetController(msg[0], msg[1]);
                        break;
                    case EVENT_VS_SETUP_CO + 0:
                    case EVENT_VS_SETUP_CO + 1:
                    case EVENT_VS_SETUP_CO + 2:
                    case EVENT_VS_SETUP_CO + 3:
                    case EVENT_VS_SETUP_CO + 4:
                    case EVENT_VS_SETUP_CO + 5:
                    case EVENT_VS_SETUP_CO + 6:
                    case EVENT_VS_SETUP_CO + 7:
                        VsSetup_SetCo(msg[0], msg[1]);
                        break;
                    case EVENT_VS_SETUP_READY:
                        VsSetup_SetPlayerReady((u8)(Link_FindSlotOfPlayer(i) - 1));
                        break;
                    case EVENT_VS_SETUP_CANCEL:
                        if (gMenuState.setup.unk_2d0[Link_FindSlotOfPlayer(i) - 1] == 0) {
                            cancel = 1;
                        } else {
                            VsSetup_ClearPlayerReady((u8)(Link_FindSlotOfPlayer(i) - 1));
                        }
                        break;
                    }
                }
            }
        }
        EventQueue_Drain();
    }
    if (cancel == 1) {
        VsSetup_Cancel();
        func_0207b48c();
        EventQueue_Reset();
        for (i = 0; i < 4; i++) {
            gMenuState.setup.unk_2d0[i] = 0;
        }
        Task_ResumeScript(task);
        return;
    }
    for (i = 0; i < 4; i++) {
        if (Link_GetConnectedSlotMask() & (1 << i)) {
            if (gMenuState.setup.unk_2d0[i] == 0) {
                allReady = 0;
                break;
            }
        }
    }
    if (allReady == 1) {
        func_02067b0c();
        func_0207b48c();
        EventQueue_Reset();
        for (i = 0; i < 4; i++) {
            gMenuState.setup.unk_2d0[i] = 0;
        }
        Task_ResumeScript(task);
        return;
    }
    if (MENU->setup.unk_2d0[Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1] == 0) {
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        EventQueue_Push(EVENT_VS_SETUP_CANCEL, 0);
    }
}
