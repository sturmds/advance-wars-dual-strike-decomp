#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

int Player_GetDefenseBonus(int playerIndex, int defenderType, int attackerType,
                           int x, int y);

/* Defense of a unit against an attacker type, in percent: 100 plus the
 * owner's defense bonus. */
int Unit_GetDefense(MapUnit* unit, int attackerType) {
  return 100 + Player_GetDefenseBonus(
                   MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], unit),
                   unit->type, attackerType, unit->x, unit->y);
}
