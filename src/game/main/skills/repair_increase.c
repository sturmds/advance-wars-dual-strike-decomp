#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

/* Skill words of the front CO of a player. */
#define FRONT_CO_SKILLS(player) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Extra repair from the Mechanic and Gear Head Force Ranks of a
 * player's front CO. slot is passed (as for CaptureIncrease) but not read. */
int RepairIncrease(int player, int slot) {
  int bonus = 0;

  if (CheckSkillsEquipped(FRONT_CO_SKILLS(player), SKILL_RANK_MECHANIC)) {
    bonus += 1;
  }
  if (CheckSkillsEquipped(FRONT_CO_SKILLS(player), SKILL_RANK_GEAR_HEAD)) {
    bonus += 2;
  }
  return bonus;
}
