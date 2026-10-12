#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMaxRange(int player, int unitType);
void RangeMap_MarkAttackTargets(int x, int y, s16 range, s8 mark);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Marks the targets a unit can attack from (x, y) within its CO-adjusted
 * range with 0. A unit that is not direct then marks the tiles inside its
 * minimum range with -1, overwriting those marks. */
void RangeMap_MarkUnitAttackRange(int x, int y, MapUnit* unit) {
  RangeMap_MarkAttackTargets(
      x, y, GetUnitMaxRange(MAP_UNIT_OWNER(STATE, unit), unit->type), 0);
  if (Ov000_gUnitStats[unit->type].maxRange != 1) {
    RangeMap_MarkAttackTargets(x, y, Ov000_gUnitStats[unit->type].minRange - 1,
                               -1);
  }
}
