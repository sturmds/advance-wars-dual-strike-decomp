#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

/* The unit list looks for transports in the first 50 slots after the
 * current player's first slot. */
#define FIRST_SLOT (Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1)
#define END_SLOT (Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51)

/* Which cargo tree piece UnitList_DrawRow puts before the unit in slot
 * (counted from the current player's first slot):
 *   0     not carried
 *   1     first of two units in a transport that is not carried itself
 *   2     last unit in a transport that is not carried itself
 *   3, 5  first of two units in a carried transport; 5 when that transport
 *         is the last unit in its own carrier
 *   4, 6  last unit in a carried transport; 6 as for 5 */
int UnitList_GetCargoBranch(int slot) {
  MapUnit* units;
  MapUnit* transport;
  MapUnit* outer;
  int i;
  int j;
  int k;
  MapUnit* carrier;
  u16 base;

  units = MAP_STATE->units;
  if (!(units[slot + MAP_STATE->currentUnitBase].flags & MAP_UNIT_CARRIED)) {
    return 0;
  }
  for (i = FIRST_SLOT; i < END_SLOT; i++) {
    transport = &units[i];
    if (transport->type == UNIT_NONE) {
      continue;
    }
    base = MAP_STATE->currentUnitBase;
    if (slot == transport->cargo[0] - base) {
      if (transport->cargo[1] != 0) {
        if (!(transport->flags & MAP_UNIT_CARRIED)) {
          return 1;
        }
        /* slot now names the transport; if no carrier holds it,
                 * the scan goes on looking for that slot instead. */
        slot = i - base;
        for (j = FIRST_SLOT; j < END_SLOT; j++) {
          outer = &MAP_STATE->units[j];
          if (outer->type == UNIT_NONE) {
            continue;
          }
          if (slot == outer->cargo[1] - base) {
            return 5;
          }
          if (slot == outer->cargo[0] - base) {
            return outer->cargo[1] == 0 ? 5 : 3;
          }
        }
        continue;
      }
    } else if (slot != transport->cargo[1] - base) {
      continue;
    }
    if (!(transport->flags & MAP_UNIT_CARRIED)) {
      return 2;
    }
    slot = i - base;
    for (k = FIRST_SLOT; k < END_SLOT; k++) {
      carrier = &MAP_STATE->units[k];
      if (carrier->type == UNIT_NONE) {
        continue;
      }
      if (slot == carrier->cargo[1] - base) {
        return 6;
      }
      if (slot == carrier->cargo[0] - base) {
        return carrier->cargo[1] == 0 ? 6 : 4;
      }
    }
  }
  return 0;
}
