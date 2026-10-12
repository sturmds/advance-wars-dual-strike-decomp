#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Clears the waited and paralysed flags of the current player's units so
 * they can act again; units inside a transport are skipped. Only the
 * first 51 of the player's 64 slots are checked. */
void Player_RefreshUnits(void) {
  int i;
  MapUnit* unit;

  for (i = STATE->currentUnitBase; i < STATE->currentUnitBase + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_CARRIED) == 0) {
      unit->flags &= ~MAP_UNIT_WAITED;
      unit->flags &= ~MAP_UNIT_PARALYSED;
    }
  }
}
