#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s8 UnitMenu_JoinHidden(void);
s8 UnitMenu_BoardHidden(void);
void RangeMap_Fill(int value);
void RangeMap_SetAdjacentReachable(int x, int y);
int RepairTargets_Build(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test: 1 hides the Repair entry. It shows for a Black Boat
 * that did not just unload (unk_1c 5), with no Join or Board entry, and a unit
 * to repair next to its destination. */
s8 UnitMenu_RepairHidden(void) {
  if (STATE->unk_1c == 5) {
    return 1;
  }
  if (UnitMenu_JoinHidden() == 0) {
    return 1;
  }
  if (UnitMenu_BoardHidden() == 0) {
    return 1;
  }
  if (STATE->actingUnit->type != UNIT_BLACK_BOAT) {
    return 1;
  }
  RangeMap_Fill(-1);
  RangeMap_SetAdjacentReachable(STATE->actingUnitDestX, STATE->actingUnitDestY);
  return RepairTargets_Build() == 0;
}
