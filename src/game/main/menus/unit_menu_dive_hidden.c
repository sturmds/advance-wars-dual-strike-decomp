#include "types.h"
#include "game/Unk_02080440.h"

s8 UnitMenu_JoinHidden(void);
s8 UnitMenu_BoardHidden(void);

/* Unit menu entry test shared by the Dive entries: 1 hides the entry. It
 * shows only for a unit that has not dived while the Join and Board entries
 * are hidden. */
s8 UnitMenu_DiveHidden(void) {
  if (MAP_STATE->actingUnit->flags & MAP_UNIT_DIVED) {
    return 1;
  }
  if (UnitMenu_JoinHidden() == 0) {
    return 1;
  }
  return UnitMenu_BoardHidden() == 0;
}
