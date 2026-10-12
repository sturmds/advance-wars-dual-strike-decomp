#include "types.h"
#include "game/unit_stats.h"

BOOL func_ov000_022b0138(int unitType) {
  switch (unitType) {
    case UNIT_BATTLESHIP:
    case UNIT_LANDER:
    case UNIT_SUB:
    case UNIT_CARRIER:
    case UNIT_OOZIUM:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
      return FALSE;
    default:
      return TRUE;
  }
}
