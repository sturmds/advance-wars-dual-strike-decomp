#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai/ai_tile_search.h"
#include "game/ai/ai_units_in_range.h"

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

/* Fills the range map from (x, y) for the unit in cargo slot 1, then counts
 * the team's foot units it reaches (plus one when the unit in slot 0 is
 * Infantry or Mech). TRUE when the armed non-foot ground units reached are
 * fewer. */
BOOL AiSeaTransport_FootUnitsOutnumberArmed(int x, int y) {
  MapUnit* units = MAP_STATE->units;
  int slot = MAP_STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];
  int footUnits;
  u8 type;

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  footUnits = Ai_CountTeamFootUnitsInRange();
  type = MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]].type;
  if (type != UNIT_NONE && type <= UNIT_MECH) {
    footUnits++;
  }
  return Ai_CountTeamArmedGroundUnitsInRange() < footUnits;
}
