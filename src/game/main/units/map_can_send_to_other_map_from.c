#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/screen_other_map.h"
#include "game/unit_stats.h"

BOOL Map_IsVacantDeployTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when a unit can be sent from (x, y) to the other map: the battle has
 * two maps, the other map's unk_1c4->unk_30 is 0, this is the first map, and
 * either unk_86 is set and the acting unit is a plane, or (x, y) is a vacant
 * deploy tile. unit is not read. */
BOOL Map_CanSendToOtherMapFrom(MapUnit* unit, int x, int y) {
  if (STATE->secondMapId == 0 || OtherMap_GetUnk30() != 0) {
    return FALSE;
  }
  if (data_027c027c != data_027c0288.unk_00) {
    return FALSE;
  }
  if (STATE->unk_86) {
    if (Ov000_gUnitStats[STATE->actingUnit->type].unitClass !=
        UNIT_CLASS_PLANE) {
      return FALSE;
    }
  } else if (!Map_IsVacantDeployTile((s16)x, (s16)y)) {
    return FALSE;
  }
  return TRUE;
}
