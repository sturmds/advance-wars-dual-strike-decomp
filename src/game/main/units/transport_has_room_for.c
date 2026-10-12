#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* TRUE when the transport can take a unit of unitType aboard: its type lists
 * unitType in carryTypes, and a free slot is left (carryTypes[0] is the
 * capacity, 1 or 2). */
BOOL Transport_HasRoomFor(MapUnit* transport, int unitType) {
  const u8* carry = Ov000_gUnitStats[transport->type].carryTypes;
  int capacity;

  if (carry == NULL) {
    return FALSE;
  }
  if ((carry + unitType)[1] == 0) {
    return FALSE;
  }
  capacity = carry[0];
  if (capacity == 2 && transport->cargo[1] != 0) {
    return FALSE;
  }
  if (capacity == 1 && transport->cargo[0] != 0) {
    return FALSE;
  }
  return TRUE;
}
