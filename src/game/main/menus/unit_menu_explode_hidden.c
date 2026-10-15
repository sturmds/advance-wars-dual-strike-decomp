#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

BOOL UnitMenu_JoinHidden(void);
BOOL UnitMenu_BoardHidden(void);

/* Unit menu entry test: 1 hides the entry. Joining and boarding take
 * priority; otherwise only a Black Bomb gets the entry. */
s8 UnitMenu_ExplodeHidden(void) {
  if (!UnitMenu_JoinHidden()) {
    return 1;
  }
  if (!UnitMenu_BoardHidden()) {
    return 1;
  }
  if (MAP_STATE->actingUnit->type != UNIT_BLACK_BOMB) {
    return 1;
  }
  return 0;
}
