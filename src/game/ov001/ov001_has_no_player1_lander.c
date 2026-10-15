#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

/* Nonzero when player 1 has no Lander. */
BOOL Ov001_HasNoPlayer1Lander(void) {
  int i;
  int type;

  for (i = Ov000_gPlayerFirstUnitSlot[1] + 1;
       i < Ov000_gPlayerFirstUnitSlot[1] + 51; i++) {
    type = MAP_STATE->units[i].type;
    /* The empty-slot test is redundant; needed to match. */
    if (type != UNIT_NONE && type == UNIT_LANDER) {
      return FALSE;
    }
  }
  return TRUE;
}
