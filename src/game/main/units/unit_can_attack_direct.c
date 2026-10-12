#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMaxRange(int player, int unitType);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit can hit an adjacent target: its type has unk_17 set
 * (attacks without ammo), or its maximum range is 1 and it has ammo left. */
BOOL Unit_CanAttackDirect(MapUnit* unit) {
  if (Ov000_gUnitStats[unit->type].unk_17 == 1) {
    return TRUE;
  }
  if (GetUnitMaxRange(MAP_UNIT_OWNER(GAME, unit), unit->type) > 1) {
    return FALSE;
  }
  return unit->ammo != 0;
}
