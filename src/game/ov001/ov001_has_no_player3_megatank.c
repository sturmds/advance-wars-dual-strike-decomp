#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

/* Nonzero when player 3 has no Megatank. */
BOOL Ov001_HasNoPlayer3Megatank(void) {
  int i;
  int type;

  for (i = Ov000_gPlayerFirstUnitSlot[3] + 1;
       i < Ov000_gPlayerFirstUnitSlot[3] + 51; i++) {
    type = MAP_STATE->units[i].type;
    /* The empty-slot test is redundant; needed to match. */
    if (type != UNIT_NONE && type == UNIT_MEGATANK) {
      return FALSE;
    }
  }
  return TRUE;
}
