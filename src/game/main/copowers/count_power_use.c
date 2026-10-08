#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Counts one more power use for the player's front CO, up to 10. The
 * count raises the cost of the next power (see PowerStarCharge). */
void CountPowerUse(int player)
{
    Player *p = &data_027c027c->unk_48[data_027c0284]->players[player];

    if (PLAYER_CO_STATUS(p->frontCo)->powerUses < 10) {
        PLAYER_CO_STATUS(p->frontCo)->powerUses++;
    }
}
