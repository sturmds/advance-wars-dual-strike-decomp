#include "types.h"
#include "game/event_queue.h"
#include "game/menu_button.h"

int Link_IsActive(void);
void VsSetup_NextTeam(int message);

/* Next team for a team screen button: a slot button (48-51) or the arrow
 * below it (53, 55, 57, 59, which also selects the slot button). In link play
 * it sends the message to the event queue, else it applies it at once. */
void VsSetup_NextTeamForButton(MenuButton *button)
{
    int slot = button->id - 48;

    switch (button->id) {
    case 48:
    case 49:
    case 50:
    case 51:
        if (Link_IsActive() == 1) {
            EventQueue_Push((u8)(slot + EVENT_VS_TEAM_NEXT), 0);
        } else {
            VsSetup_NextTeam((u8)(slot + EVENT_VS_TEAM_NEXT));
        }
        break;
    case 53:
    case 55:
    case 57:
    case 59:
        slot = (button->id - 53) >> 1;
        MenuButton_Select(slot + 48);
        if (Link_IsActive() == 1) {
            EventQueue_Push((u8)(slot + EVENT_VS_TEAM_NEXT), 0);
        } else {
            VsSetup_NextTeam((u8)(slot + EVENT_VS_TEAM_NEXT));
        }
        break;
    }
}
