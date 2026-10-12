#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Skill words of one CO of a player: slot 0 front, 1 back. */
#define CO_SKILLS(player, slot) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[slot])

/* Unit cost change in percent from the Force Ranks of one CO (slot 0
 * front, 1 back) of a player. Zero or negative. */
int CostReduced(int player, int slot) {
  int percent = 0;

  if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_SALE_PRICE)) {
    percent -= 5;
  }
  if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_FIRE_SALE)) {
    percent -= 8;
  }
  return percent;
}
