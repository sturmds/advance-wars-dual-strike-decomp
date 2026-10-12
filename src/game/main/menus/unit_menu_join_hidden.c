#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Unit_CanJoin(MapUnit* unit, MapUnit* other);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test: 1 hides the entry. Join is shown when a unit stands on
 * the destination tile and the acting unit can join it. The Explode, Capture
 * and Board tests hide their entries while this one shows. */
s8 UnitMenu_JoinHidden(void) {
  u8 slot;

  slot = STATE->mapTiles
             ->unitAt[STATE->actingUnitDestX +
                      STATE->mapTiles->rowStart[STATE->actingUnitDestY]];
  if (slot == 0) {
    return 1;
  }
  return !Unit_CanJoin(STATE->actingUnit, &STATE->units[slot]);
}
