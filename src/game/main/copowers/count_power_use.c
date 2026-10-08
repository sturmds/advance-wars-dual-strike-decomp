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
    u32 uses = PLAYER_POWER_USES(p->frontCo);

    if (uses < 10) {
        p->frontCo = (p->frontCo & ~PLAYER_POWER_USES_MASK) | (((uses + 1) & 0xf) << PLAYER_POWER_USES_SHIFT);
    }
}
