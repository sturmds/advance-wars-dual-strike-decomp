#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

/* Megatank, Piperunner, Oozium or type 30 (not in Ov000_gUnitStats). What these have in
 * common is unknown. */
BOOL func_ov002_02354670(int side) {
  switch (BATTLE_SIDE_UNIT_TYPE(side)) {
    case UNIT_MEGATANK:
    case UNIT_PIPERUNNER:
    case UNIT_OOZIUM:
    case 30:
      return TRUE;
    default:
      return FALSE;
  }
}
