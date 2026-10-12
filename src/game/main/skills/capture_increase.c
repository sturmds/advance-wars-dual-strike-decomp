#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Skill words of one CO of a player: slot 0 front, 1 back. */
#define CO_SKILLS(player, slot) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[slot])

/* Capture bonus from the Invader and Conqueror Force Ranks of one CO
 * (slot 0 front, 1 back) of a player. */
int CaptureIncrease(int player, int slot) {
  int bonus = 0;

  if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_INVADER)) {
    bonus += 1;
  }
  if (CheckSkillsEquipped(CO_SKILLS(player, slot), SKILL_RANK_CONQUEROR)) {
    bonus += 2;
  }
  return bonus;
}
