#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"

int GetUnitCombatClass(int unitType);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Skill words of one CO of a player: slot 0 front, 1 back. */
#define CO_SKILLS(player, slot) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[slot])

/* Firepower bonus from the skills and Force Ranks of one CO (slot 0
 * front, 1 back) of a player, for an attacker of the given unit type. */
int FirepowerIncrease(int player, int slot, int unitType) {
  int bonus = 0;

  if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_PARTNER_TEAMWORK)) {
    bonus += 5;
  }
  if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_PARTNER_SYNERGY)) {
    bonus += 8;
  }

  switch (GetUnitCombatClass(unitType)) {
    case UNIT_CLASS_DIRECT_COMBAT:
      if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_BRUISER)) {
        bonus += 5;
      }
      if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_BRAWLER)) {
        bonus += 8;
      }
      break;
    case UNIT_CLASS_INDIRECT_COMBAT:
      if (CheckSkillsEquipped(CO_SKILLS(player, slot),
                              SKILL_RANK_SHARPSHOOTER)) {
        bonus += 5;
      }
      if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_SNIPER)) {
        bonus += 8;
      }
      break;
  }

  switch (data_027c027c->unk_48[data_027c0284]->unk_1c4->weather) {
    case WEATHER_SNOW:
      if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_ICEBREAKER)) {
        bonus += 20;
      }
      if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_SNOW_FIREPOWER)) {
        bonus += 20;
      }
      break;
    case WEATHER_RAIN:
      if (CheckSkillsEquipped(CO_SKILLS(player, slot),
                              SKILL_RANK_HIGH_AND_DRY)) {
        bonus += 20;
      }
      break;
    case WEATHER_SAND:
      if (CheckSkillsEquipped(CO_SKILLS(player, slot),
                              SKILL_RANK_SAND_SCORPION)) {
        bonus += 20;
      }
      break;
  }

  return bonus;
}
