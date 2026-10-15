#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"
#include "game/unit_stats.h"

#define FRONT_SKILLS(player) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Change to the daily fuel use of a unit type from the player's front CO:
 * -2 for planes and copters with SKILL_AIR_FUEL_SAVING (Eagle), else 0. */
int Player_GetAirFuelChange(int player, int unitType) {
  u8 unitClass;

  if (CheckSkillsEquipped(FRONT_SKILLS(player), SKILL_AIR_FUEL_SAVING)) {
    unitClass = Ov000_gUnitStats[unitType].unitClass;
    /* Range check for PLANE..COPTER, as in UnitHighlightAirUnits. */
    unitClass += (u8)-UNIT_CLASS_PLANE;
    return unitClass <= UNIT_CLASS_COPTER - UNIT_CLASS_PLANE ? -2 : 0;
  }
  return 0;
}
