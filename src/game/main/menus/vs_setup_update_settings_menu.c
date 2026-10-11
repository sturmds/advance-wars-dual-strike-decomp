#include "types.h"
#include "game/event_queue.h"
#include "game/task.h"

u8 *func_02078138(int player);
void func_0207b48c(void);
u8 Link_GetOwnPlayer(void);
u32 Link_IsActive(void);
int func_0207bd94(void);
int func_02064e00(int row);
void func_020651ec(void);
void func_02065214(void);
void VsSetup_IncreaseSetting(void);
void VsSetup_DecreaseSetting(void);
void VsSetup_HandleSettingsKeys(void);
void Task_ResumeScript(Task *task);

/* Player 0 hosts in link play. Needed to match: the flag is built as a
 * BOOL, then compared with TRUE. */
static inline BOOL IsLinkHost(void)
{
    BOOL host;

    if (Link_GetOwnPlayer() == 0) {
        host = TRUE;
    } else {
        host = FALSE;
    }
    return host;
}

/* Task step of the Vs setup settings menu. Outside link play, and on the
 * link host, VsSetup_HandleSettingsKeys handles the keys. In link play
 * every machine then applies the host's messages from the event queue, so all screens
 * change together; confirm or cancel closes the menu and resumes the
 * task's script. */
void VsSetup_UpdateSettingsMenu(Task *task)
{
    int confirm;
    int cancel;
    u8 *msg;
    int i;

    confirm = 0;
    cancel = 0;
    if (!Link_IsActive() || IsLinkHost() == TRUE) {
        VsSetup_HandleSettingsKeys();
    }
    if (Link_IsActive() != 1) {
        return;
    }
    if (func_0207bd94() == 1) {
        msg = func_02078138(0);
        for (i = 0; i < 4; i++, msg += 2) {
            switch (msg[0]) {
            case EVENT_VS_SETTING_SELECT_ROW:
                func_02064e00(msg[1]);
                break;
            case EVENT_VS_SETTING_INCREASE:
                VsSetup_IncreaseSetting();
                break;
            case EVENT_VS_SETTING_DECREASE:
                VsSetup_DecreaseSetting();
                break;
            case EVENT_VS_SETTINGS_CONFIRM:
                confirm = TRUE;
                break;
            case EVENT_VS_SETTINGS_CANCEL:
                cancel = TRUE;
                break;
            }
        }
        EventQueue_Drain();
    }
    if (confirm == TRUE) {
        func_02065214();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
        return;
    }
    if (cancel == TRUE) {
        func_020651ec();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
    }
}
