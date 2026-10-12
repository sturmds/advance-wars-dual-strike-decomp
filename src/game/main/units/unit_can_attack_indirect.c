#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMaxRange(int player, int unitType);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit has a ranged weapon (maximum range above 1 for its
 * owner's CO) and ammo left. */
BOOL Unit_CanAttackIndirect(MapUnit* unit) {
  if (GetUnitMaxRange(MAP_UNIT_OWNER(GAME, unit), unit->type) == 1) {
    return FALSE;
  }
  return unit->ammo != 0;
}
