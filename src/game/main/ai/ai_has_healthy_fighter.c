#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the current player has a Fighter with at least half hp (hp 100 is
 * full). The scan skips the player's first unit slot and covers the next 50.
 * The Fighter builder does not build another one while this holds. */
BOOL Ai_HasHealthyFighter(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type == UNIT_FIGHTER &&
        unit->hp >= 50) {
      return TRUE;
    }
  }
  return FALSE;
}
