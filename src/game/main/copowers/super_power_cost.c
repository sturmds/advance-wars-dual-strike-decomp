#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int PowerStarCharge(int player, int slot);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Meter charge a Super CO Power costs for the CO in a player's slot (0
 * front, 1 back): the CO's Super star count times the charge per star. */
int SuperPowerCost(int player, int slot)
{
    int starCharge = PowerStarCharge(player, slot);
    /* needed to match: the CO id is read with a byte load */
    u8 co = *(u8 *)&(&data_027c027c->unk_48[data_027c0284]->players[player].frontCo)[slot];

    /* needed to match: the game reads superStars as a u32 and indexes
     * from &gCoTable[0].superStars, so the literal pool holds gCoTable+0x20 */
    return starCharge * *(u32 *)&((CoRecord *)&gCoTable[0].superStars)[co & PLAYER_CO_ID_MASK];
}
