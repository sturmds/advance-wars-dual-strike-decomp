#include "types.h"
#include "game/menu_state.h"

void func_02085c2c(int player, u8 row);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Link message 12: a player who is not ready moves to the previous row
 * (before row 1 comes row 4) that has fewer than 7 members. */
void CombatSetup_MoveToPrevRow(int player)
{
    u8 ready = MENU->combat.players[player].unk_1;
    u8 row = MENU->combat.players[player].row;

    if (ready != 0) {
        return;
    }
    do {
        if (row == 1) {
            row = 4;
        } else {
            row--;
        }
    } while (MENU->combat.rows[row - 1].unk_07 == 7);
    func_02085c2c(player, row);
}
