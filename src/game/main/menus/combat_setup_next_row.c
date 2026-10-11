#include "types.h"
#include "game/menu_state.h"

void CombatSetup_MovePlayerToRow(int player, u8 row);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Link message 13: a player who is not ready moves to the next row (4 is
 * followed by 1) that has fewer than 7 members. */
void CombatSetup_MoveToNextRow(int player)
{
    u8 ready = MENU->combat.players[player].ready;
    u8 row = MENU->combat.players[player].row;

    if (ready != 0) {
        return;
    }
    do {
        if (row == 4) {
            row = 1;
        } else {
            row++;
        }
    } while (MENU->combat.rows[row - 1].memberCount == 7);
    CombatSetup_MovePlayerToRow(player, row);
}
