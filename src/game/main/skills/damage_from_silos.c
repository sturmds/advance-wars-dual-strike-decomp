#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

/* Reduction of the damage a player's units take from missile silos, from
 * the skills of one CO (coSlot 0 front, 1 back): 10 with Missile Guard. */
int DamageFromSilos(int playerIndex, int coSlot) {
  Player* player = &data_027c027c->unk_48[data_027c0284]->players[playerIndex];
  int reduction = 0;

  if (CheckSkillsEquipped(player->skills[coSlot], SKILL_RANK_MISSILE_GUARD)) {
    reduction += 10;
  }
  return reduction;
}
