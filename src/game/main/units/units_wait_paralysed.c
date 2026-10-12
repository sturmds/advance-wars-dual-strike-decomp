#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Marks every paralysed unit of the current player that is not aboard a
 * transport as waited, so it cannot act. Covers slots 1 to 50 of the block. */
void Units_WaitParalysed(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_CARRIED) == 0) {
      if (unit->flags & MAP_UNIT_PARALYSED) {
        unit->flags |= MAP_UNIT_WAITED;
      }
    }
  }
}
