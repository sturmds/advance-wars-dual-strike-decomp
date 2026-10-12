#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"

int GetUnitCombatClass(int unitType);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Skill words of the front CO of a player. */
#define FRONT_CO_SKILLS(player) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Defense bonus from the skills and Force Ranks of a player's front CO,
 * by the class of the attacking unit. Only player and attackerType are
 * read; the caller passes the defender's type and two more unit bytes. */
int DefenseIncreaseDirectIndirect(int player, int defenderType,
                                  int attackerType, int arg3, int arg4) {
  int bonus = 0;

  switch (GetUnitCombatClass(attackerType)) {
    case UNIT_CLASS_DIRECT_COMBAT:
      if (CheckSkillsEquipped(FRONT_CO_SKILLS(player), SKILL_RANK_SLAM_GUARD)) {
        bonus += 8;
      }
      if (CheckSkillsEquipped(FRONT_CO_SKILLS(player),
                              SKILL_RANK_SLAM_SHIELD)) {
        bonus += 12;
      }
      break;
    case UNIT_CLASS_INDIRECT_COMBAT:
      if (CheckSkillsEquipped(FRONT_CO_SKILLS(player),
                              SKILL_RANK_SNIPE_GUARD)) {
        bonus += 8;
      }
      if (CheckSkillsEquipped(FRONT_CO_SKILLS(player),
                              SKILL_RANK_SNIPE_SHIELD)) {
        bonus += 12;
      }
      if (CheckSkillsEquipped(FRONT_CO_SKILLS(player),
                              SKILL_INDIRECT_DEFENSE_20)) {
        bonus += 20;
      }
      if (CheckSkillsEquipped(FRONT_CO_SKILLS(player),
                              SKILL_INDIRECT_DEFENSE_40)) {
        bonus += 40;
      }
      if (CheckSkillsEquipped(FRONT_CO_SKILLS(player),
                              SKILL_INDIRECT_DEFENSE_80)) {
        bonus += 80;
      }
      break;
  }

  return bonus;
}
