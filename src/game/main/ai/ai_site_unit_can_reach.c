#include "types.h"
#include "game/ai_site.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when a unit counts as able to reach the site. At a base, a ground unit
 * must stand on a tile the range map reaches and a ship must carry cargo; at
 * a port, ground units never count. Air units always do. */
BOOL AiSite_UnitCanReach(int siteIndex, MapUnit* unit) {
  u8 moveGroup;

  if (data_0229e5bc[siteIndex].type == SITE_BASE) {
    moveGroup = Ov000_gUnitStats[unit->type].moveGroup;
    if (moveGroup & (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
      if (STATE->rangeMap[unit->y][unit->x] < 0) {
        return FALSE;
      }
    }
    if (moveGroup & MOVE_GROUP_SEA) {
      if (unit->cargo[0] == 0) {
        return FALSE;
      }
    }
  }
  if (data_0229e5bc[siteIndex].type == SITE_PORT) {
    if (Ov000_gUnitStats[unit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
      return FALSE;
    }
  }
  return TRUE;
}
