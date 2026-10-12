#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define SEARCH_RANGE_MAX 110

/* Same as AiUnit_GetSearchMovement, for the acting unit. A unit that is
 * not a foot unit counts its moves this turn (gAiTurnState->unk_05). The
 * name is a guess too. */
int AiActingUnit_GetSearchMovement(void) {
  MapUnit* unit;
  int range;

  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return SEARCH_RANGE_MAX;
  }
  unit = STATE->actingUnit;
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
    range = gAiTurnState->unk_05;
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
