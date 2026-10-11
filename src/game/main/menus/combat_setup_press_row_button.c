#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void CombatSetup_PushMoveToRow(int row);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Row buttons are ids 16-19 for rows 1-4 (CombatSetup_AddRowButtons). Sends
 * message 11 with the button's row unless the local player is ready or is
 * already on that row. */
void CombatSetup_PressRowButton(MenuButton *button)
{
    if (MENU->combat.players[data_0229fb30.unk_45a].ready != 0) {
        return;
    }
    if ((u8)(button->id - 15) == MENU->combat.players[data_0229fb30.unk_45a].row) {
        return;
    }
    CombatSetup_PushMoveToRow((u8)(button->id - 15));
}
