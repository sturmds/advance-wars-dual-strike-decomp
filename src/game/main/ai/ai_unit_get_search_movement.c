#include "types.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

int Unit_GetUsableMovement(MapUnit* unit);

#define SEARCH_RANGE_MAX 110

/* Movement budget the AI gives the move-range fill for a unit. 110 (no limit
 * in practice) when the player has no sea domain, the unit is not a ground
 * unit, or its claimedLandmass flag is set. Otherwise a foot unit counts 4, other
 * ground units their usable movement, scaled by a gAiProfile factor picked by
 * unk_0c_4, capped at 110. The purpose is a guess: callers use it for capture
 * targets, and probably allow for ferrying by sea. */
int AiUnit_GetSearchMovement(MapUnit* unit) {
  int range;

  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return SEARCH_RANGE_MAX;
  }
  if (!(Ov000_gUnitStats[unit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
    return SEARCH_RANGE_MAX;
  }
  if (unit->claimedLandmass) {
    return SEARCH_RANGE_MAX;
  }
  if (unit->type <= UNIT_MECH) {
    range = 4;
  } else {
    range = Unit_GetUsableMovement(unit);
  }
  if (unit->unk_0c_4) {
    range = range * gAiProfile->unk_08[7];
  } else {
    range = range * gAiProfile->unk_08[6];
  }
  if (range > SEARCH_RANGE_MAX) {
    range = SEARCH_RANGE_MAX;
  }
  return range;
}
