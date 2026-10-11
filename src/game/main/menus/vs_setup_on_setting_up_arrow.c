#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
int VsSetup_SelectSettingRow(int row);
void VsSetup_IncreaseSetting(void);

/* Touch on the arrow above a setting row (button 69 + row * 2): selects the
 * row, then raises its value. */
void VsSetup_OnSettingUpArrow(u8 *buttonId)
{
    if (Link_IsActive() == 1) {
        EventQueue_Push(EVENT_VS_SETTING_SELECT_ROW, (u8)((*buttonId - 69) >> 1));
    } else {
        VsSetup_SelectSettingRow((u8)((*buttonId - 69) >> 1));
    }
    if (Link_IsActive() == 1) {
        EventQueue_Push(EVENT_VS_SETTING_INCREASE, 0);
    } else {
        VsSetup_IncreaseSetting();
    }
}
