#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s8 UnitMenu_JoinHidden(void);
s8 UnitMenu_BoardHidden(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test shared by the dived-unit Emerge entries: 1 hides the
 * entry. It shows only for a dived unit while the Join and Board entries are
 * hidden. */
s8 UnitMenu_EmergeHidden(void) {
  if (UnitMenu_JoinHidden() == 0) {
    return 1;
  }
  if (UnitMenu_BoardHidden() == 0) {
    return 1;
  }
  return (STATE->actingUnit->flags & MAP_UNIT_DIVED) == 0;
}
