#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL AiTransport_SecondCargoReachesTargetAt(int x, int y);
BOOL func_020b4da8(int x, int y);
BOOL AiSeaTransport_FootUnitsOutnumberArmed(int x, int y);
BOOL func_020b50bc(int x, int y);
BOOL func_020b51ec(int x, int y);
BOOL AiSeaTransport_FewArmedUnitsInRange(int x, int y);
BOOL AiSeaTransport_FewFootUnitsForProperties(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Called by AiTransport_UnloadAt when the acting transport stops at (x, y) to
 * unload: TRUE when its second cargo unit should get off there too. The
 * unit's own order picks the check, like AiSeaTransport_MoveLoaded picks
 * a routine (no check exists for order 7: the original calls NULL). If
 * that check fails and the order has no fixed target (not 8, 9 or 10),
 * the checks for orders 0, 3, 4 and 2 are tried in turn; the first that
 * passes becomes the unit's order and clears its target. */
BOOL AiTransport_SecondCargoHasTaskAt(int x, int y) {
  BOOL (*check)(int, int);
  MapUnit* unit = &STATE->units[STATE->actingUnit->cargo[1]];

  if (unit->type <= UNIT_MECH || unit->type == UNIT_APC) {
    check = AiSeaTransport_FewFootUnitsForProperties;
  } else {
    switch (unit->aiOrder) {
      case 0:
      case 1:
        check = AiSeaTransport_FewArmedUnitsInRange;
        break;
      case 2:
        check = func_020b51ec;
        break;
      case 3:
      case 5:
        check = func_020b50bc;
        break;
      case 4:
        check = AiSeaTransport_FootUnitsOutnumberArmed;
        break;
      case 6:
        check = func_020b4da8;
        break;
      case 8:
      case 9:
      case 10:
        check = AiTransport_SecondCargoReachesTargetAt;
        break;
      default:
        check = NULL;
        break;
    }
  }
  if (check(x, y) == TRUE) {
    return TRUE;
  }
  switch (unit->aiOrder) {
    case 8:
    case 9:
    case 10:
      return FALSE;
  }
  if (AiSeaTransport_FewArmedUnitsInRange(x, y) == TRUE) {
    unit->aiOrder = 0;
    unit->aiTargetX = MAP_UNIT_NO_TARGET;
    unit->aiTargetY = MAP_UNIT_NO_TARGET;
    return TRUE;
  }
  if (func_020b50bc(x, y) == TRUE) {
    unit->aiOrder = 3;
    unit->aiTargetX = MAP_UNIT_NO_TARGET;
    unit->aiTargetY = MAP_UNIT_NO_TARGET;
    return TRUE;
  }
  if (AiSeaTransport_FootUnitsOutnumberArmed(x, y) == TRUE) {
    unit->aiOrder = 4;
    unit->aiTargetX = MAP_UNIT_NO_TARGET;
    unit->aiTargetY = MAP_UNIT_NO_TARGET;
    return TRUE;
  }
  if (func_020b51ec(x, y) != TRUE) {
    return FALSE;
  }
  unit->aiOrder = 2;
  unit->aiTargetX = MAP_UNIT_NO_TARGET;
  unit->aiTargetY = MAP_UNIT_NO_TARGET;
  return TRUE;
}
