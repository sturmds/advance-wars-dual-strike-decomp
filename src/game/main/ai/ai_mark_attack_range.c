#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(s8 value);
int UnitType_GetAttackKind(int unitType);
int GetUnitMaxRange(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills the range map with the acting unit's moves, then marks the tiles
 * around the reachable ones for each step of its attack range, with values
 * 111 and up. Indirect units use the CO-adjusted maximum range; the others
 * use minRange, which is 1 for a direct unit. */
void AiActingUnit_MarkAttackRange(int movement) {
  int i;
  int range;

  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, movement, 0);
  if (UnitType_GetAttackKind(STATE->actingUnit->type) == ATTACK_KIND_INDIRECT) {
    range = GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
  } else {
    range = Ov000_gUnitStats[STATE->actingUnit->type].minRange;
  }
  for (i = 0; i < range; i++) {
    RangeMap_MarkAdjacentTiles(111 + i);
  }
}
