#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

/* Firepower per Com Tower added by the skills of one CO (coSlot 0 front,
 * 1 back) of a player. The caller multiplies it by the tower count. */
int TowerFirepower(int playerIndex, int coSlot) {
  Player* player = &data_027c027c->unk_48[data_027c0284]->players[playerIndex];
  int firepower = 0;

  if (CheckSkillsEquipped(player->skills[coSlot], SKILL_RANK_TOWER_POWER)) {
    firepower += 5;
  }
  return firepower;
}
