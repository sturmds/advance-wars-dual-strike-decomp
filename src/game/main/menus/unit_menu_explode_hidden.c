#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL UnitMenu_JoinHidden(void);
BOOL UnitMenu_BoardHidden(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test: 1 hides the entry. Joining and boarding take
 * priority; otherwise only a Black Bomb gets the entry. */
s8 UnitMenu_ExplodeHidden(void) {
  if (!UnitMenu_JoinHidden()) {
    return 1;
  }
  if (!UnitMenu_BoardHidden()) {
    return 1;
  }
  if (STATE->actingUnit->type != UNIT_BLACK_BOMB) {
    return 1;
  }
  return 0;
}
