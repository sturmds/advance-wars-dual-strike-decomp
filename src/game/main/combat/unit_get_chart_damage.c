#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int func_020e5c40(int playerIndex, int slot, int powerState, int unitType); /* CO firepower, percent */
int func_020c6708(int targetType, BOOL targetDived);                         /* damage chart column */
int TerrainAndStealthFirepower(int playerIndex, int unitType, int column, int x, int y, BOOL dived);
/* chart damage * firepower / 100, at least 1 when the weapon can hit */
int func_020dfc04(int firepower, int unitType, int targetType, int weapon, BOOL targetDived);

/* Damage a unit's weapon (0 primary, 1 secondary) deals to a target unit
 * type: the damage chart value times the attacker's firepower in percent
 * (100 plus CO, terrain and stealth bonuses). targetDived picks the chart
 * column for a dived Stealth or Sub. */
int Unit_GetChartDamage(MapUnit *unit, int targetType, int weapon, BOOL targetDived)
{
    Unk_02080440_48 *state = data_027c027c->unk_48[data_027c0284];
    int player = MAP_UNIT_OWNER(state, unit);
    int unitType = unit->type;
    int firepower = func_020e5c40(player, 0, state->players[player].front.powerState, unitType) + 100;
    BOOL dived = (unit->flags & MAP_UNIT_DIVED) ? TRUE : FALSE;

    firepower += TerrainAndStealthFirepower(player, unitType, func_020c6708(targetType, targetDived), unit->x, unit->y, dived);
    return func_020dfc04(firepower, unitType, targetType, weapon, targetDived);
}
