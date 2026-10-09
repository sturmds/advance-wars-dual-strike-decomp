#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02290718.h"

#define DAMAGE_STAT_MAX 999999999

int Unit_GetValuePerHp(int playerIndex, int unitType);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_02290718 data_02290718;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Adds the funds value of hpLost hp of unit (hp times its value per hp) to
 * its owner's damage taken, and for a human owner outside game mode 4 also
 * to humanDamageTaken. Both totals stop at 999999999. */
void Unit_AddDamageTaken(MapUnit *unit, int hpLost)
{
    int owner = MAP_UNIT_OWNER(STATE, unit);
    u32 value = hpLost * Unit_GetValuePerHp(owner, unit->type);
    u32 total;

    STATE->players[owner].unk_0c += value;
    if (STATE->players[owner].unk_0c > DAMAGE_STAT_MAX) {
        STATE->players[owner].unk_0c = DAMAGE_STAT_MAX;
    }
    if (STATE->players[owner].controller != PLAYER_CONTROLLER_HUMAN) {
        return;
    }
    if (STATE->gameMode == 4) {
        return;
    }
    total = data_02290718.humanDamageTaken + value;
    if (total < DAMAGE_STAT_MAX) {
        data_02290718.humanDamageTaken = total;
    } else {
        data_02290718.humanDamageTaken = DAMAGE_STAT_MAX;
    }
}
