#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_Fill(int value);
void RangeMap_FillAttackRange(int x, int y, MapUnit* unit);
BOOL FireTargets_Build(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test for the second Fire entry (table at 0x02166070).
 * 1 hides the entry. It shows only for an indirect unit that has not moved,
 * with no other unit on its destination tile, when FireTargets_Build finds
 * nothing to fire on in its attack range. Guess: 2 shows the entry greyed out. */
s8 UnitMenu_FireBlockedHidden(void) {
  u8 minRange;

  minRange = Ov000_gUnitStats[STATE->actingUnit->type].minRange;
  if (minRange == ATTACK_DIRECT) {
    return 1;
  }
  if (STATE->mapTiles
          ->unitAt[STATE->actingUnitDestX +
                   STATE->mapTiles->rowStart[STATE->actingUnitDestY]] != 0) {
    return 1;
  }
  if (STATE->actingUnitDestX != STATE->actingUnitStartX ||
      STATE->actingUnitDestY != STATE->actingUnitStartY) {
    return 1;
  }
  if (minRange == ATTACK_NONE) {
    return 1;
  }
  RangeMap_Fill(-1);
  RangeMap_FillAttackRange(STATE->actingUnitDestX, STATE->actingUnitDestY,
                           STATE->actingUnit);
  if (FireTargets_Build()) {
    return 1;
  }
  return 2;
}
