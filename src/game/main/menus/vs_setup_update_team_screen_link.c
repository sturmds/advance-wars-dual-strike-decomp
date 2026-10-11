#include "types.h"
#include "game/event_queue.h"
#include "game/keypad.h"
#include "game/menu_state.h"
#include "game/task.h"

u16 Wireless_GetChildAids(void);
u8 *func_02078138(int player);
void func_0207b48c(void);
int Link_GetOwnPlayer(void);
int Link_IsActive(void);
int func_0207bd94(void);
int Link_FindSlotOfPlayer(int player);
u32 Link_GetConnectedSlotMask(void);
void VsSetup_PrevTeam(int msg); /* previous team for slot msg - 19 */
void VsSetup_NextTeam(int msg); /* next team for slot msg - 23 */
void VsSetup_SetSlotReady(int slot); /* sets slot ready */
void VsSetup_ClearSlotReady(int slot); /* clears slot ready */
void *VsSetup_ConfirmTeams(void);
void *func_0206664c(void);
void Task_ResumeScript(Task *task);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Link task step of the Vs setup team screen. Applies the team messages
 * of every connected machine (setup.ready holds each slot's ready
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
    if (Link_IsActive() != 1) {
        return;
    }
    if (func_0207bd94() == 1) {
        for (i = 0; i < 4; i++) {
            if ((Wireless_GetChildAids() | 1) & (1 << i)) {
                msg = func_02078138((u16)i);
                for (j = 0; j < 4; j++, msg += 2) {
                    switch (msg[0]) {
                    case EVENT_VS_TEAM_PREV + 0:
                    case EVENT_VS_TEAM_PREV + 1:
                    case EVENT_VS_TEAM_PREV + 2:
                    case EVENT_VS_TEAM_PREV + 3:
                        VsSetup_PrevTeam(msg[0]);
                        break;
                    case EVENT_VS_TEAM_NEXT + 0:
                    case EVENT_VS_TEAM_NEXT + 1:
                    case EVENT_VS_TEAM_NEXT + 2:
                    case EVENT_VS_TEAM_NEXT + 3:
                        VsSetup_NextTeam(msg[0]);
                        break;
                    case EVENT_VS_TEAM_READY:
                        VsSetup_SetSlotReady((u8)(Link_FindSlotOfPlayer(i) - 1));
                        break;
                    case EVENT_VS_TEAM_BACK:
                        if (gMenuState.setup.ready[Link_FindSlotOfPlayer(i) - 1] == 0) {
                            cancel = 1;
                        } else {
                            VsSetup_ClearSlotReady((u8)(Link_FindSlotOfPlayer(i) - 1));
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
            gMenuState.setup.ready[i] = 0;
        }
        Task_ResumeScript(task);
        return;
    }
    for (i = 0; i < 4; i++) {
        if (Link_GetConnectedSlotMask() & (1 << i)) {
            if (gMenuState.setup.ready[i] == 0) {
                allReady = 0;
                break;
            }
        }
    }
    if (allReady == 1) {
        VsSetup_ConfirmTeams();
        func_0207b48c();
        EventQueue_Reset();
        for (i = 0; i < 4; i++) {
            gMenuState.setup.ready[i] = 0;
        }
        Task_ResumeScript(task);
        return;
    }
    if (MENU->setup.ready[Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1] == 0) {
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        EventQueue_Push(EVENT_VS_TEAM_BACK, 0);
    }
}
