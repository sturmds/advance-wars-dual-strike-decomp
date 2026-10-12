#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
int VsSetup_SelectSettingRow(int row);
void VsSetup_DecreaseSetting(void);

/* Touch on the arrow below a setting row (button 70 + row * 2): selects the
 * row, then lowers its value. */
void VsSetup_OnSettingDownArrow(u8* buttonId) {
  if (Link_IsActive() == 1) {
    EventQueue_Push(EVENT_VS_SETTING_SELECT_ROW, (u8)((*buttonId - 70) >> 1));
  } else {
    VsSetup_SelectSettingRow((u8)((*buttonId - 70) >> 1));
  }
  if (Link_IsActive() == 1) {
    EventQueue_Push(EVENT_VS_SETTING_DECREASE, 0);
  } else {
    VsSetup_DecreaseSetting();
  }
}
