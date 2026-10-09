#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

void func_020c74dc(int player);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts a new unit of unitType for a player. Unit_Create calls it, and
 * battle start calls it for each unit already on the map. The count stops
 * at 255. */
void Player_CountUnitBuilt(int player, int unitType)
{
    Player *players;

    func_020c74dc(player);
    players = STATE->players;
    if (players[player].unitsBuilt[unitType] != 255) {
        players[player].unitsBuilt[unitType]++;
    }
}
