#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
int func_02064e00(int row);

/* Touch on a setting row button (id 60 + row): selects the row. */
void VsSetup_OnSettingRowButton(u8 *buttonId)
{
    if (Link_IsActive() == 1) {
        EventQueue_Push(EVENT_VS_SETTING_SELECT_ROW, (u8)(*buttonId - 60));
    } else {
        func_02064e00((u8)(*buttonId - 60));
    }
}
