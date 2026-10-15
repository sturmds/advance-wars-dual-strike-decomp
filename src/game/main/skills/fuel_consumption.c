#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"
#include "game/unit_stats.h"

/* Front CO skill words of a player. */
#define FRONT_SKILLS(player) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Fuel saved per day by the skills of a player's front CO, for a unit of
 * the given type. The caller subtracts it from the daily fuel use. hidden
 * is set for a hidden Stealth or a dived Sub. */
int FuelConsumption(int player, int unitType, int hidden) {
  int saving = 0;

  if (CheckSkillsEquipped(FRONT_SKILLS(player), SKILL_AIR_FUEL_SAVING)) {
    u8 unitClass = Ov000_gUnitStats[unitType].unitClass;

    if (unitClass == UNIT_CLASS_PLANE || unitClass == UNIT_CLASS_COPTER) {
      saving += 2;
    }
  }

  if (hidden) {
    if (CheckSkillsEquipped(FRONT_SKILLS(player), SKILL_RANK_SNEAKY)) {
      saving += 1;
    }
    if (CheckSkillsEquipped(FRONT_SKILLS(player), SKILL_RANK_STEALTHY)) {
      saving += 2;
    }
  }

  return saving;
}
