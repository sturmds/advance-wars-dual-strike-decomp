#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

/* Good luck added by the skills of one CO (coSlot 0 front, 1 back) of a
 * player. */
int LuckDamage(int playerIndex, int coSlot) {
  Player* player = &data_027c027c->unk_48[data_027c0284]->players[playerIndex];
  int luck = 0;

  if (CheckSkillsEquipped(player->skills[coSlot], SKILL_RANK_LUCK)) {
    luck += 10;
  }
  return luck;
}
