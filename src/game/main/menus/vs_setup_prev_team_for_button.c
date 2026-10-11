#include "types.h"
#include "game/event_queue.h"
#include "game/menu_button.h"

int Link_IsActive(void);
void VsSetup_PrevTeam(int message);

/* Like VsSetup_NextTeamForButton, for the previous team and the arrows above
 * the slot buttons (52, 54, 56, 58). */
void VsSetup_PrevTeamForButton(MenuButton *button)
{
    int slot = button->id - 48;

    switch (button->id) {
    case 48:
    case 49:
    case 50:
    case 51:
        if (Link_IsActive() == 1) {
            EventQueue_Push((u8)(slot + EVENT_VS_TEAM_PREV), 0);
        } else {
            VsSetup_PrevTeam((u8)(slot + EVENT_VS_TEAM_PREV));
        }
        break;
    case 52:
    case 54:
    case 56:
    case 58:
        slot = (button->id - 52) >> 1;
        MenuButton_Select(slot + 48);
        if (Link_IsActive() == 1) {
            EventQueue_Push((u8)(slot + EVENT_VS_TEAM_PREV), 0);
        } else {
            VsSetup_PrevTeam((u8)(slot + EVENT_VS_TEAM_PREV));
        }
        break;
    }
}
