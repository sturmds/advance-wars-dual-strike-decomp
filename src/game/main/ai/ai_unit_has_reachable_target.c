#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

/* TRUE when the acting unit has a target tile and the range map reaches it. */
BOOL AiUnit_HasReachableTarget(void) {
  MapUnit* unit = STATE->actingUnit;

  if (unit->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  return STATE->rangeMap[unit->aiTargetY][unit->aiTargetX] >= 0;
}
