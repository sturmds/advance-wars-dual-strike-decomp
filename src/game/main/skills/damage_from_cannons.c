#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Reduction of the damage a player's units take from cannons, from the
 * skills of one CO (coSlot 0 front, 1 back): 20 with Cannon Guard. The
 * caller (Cannon_AddBestTarget) subtracts it. */
int DamageFromCannons(int playerIndex, int coSlot) {
  Player* player = &data_027c027c->unk_48[data_027c0284]->players[playerIndex];
  int reduction = 0;

  if (CheckSkillsEquipped(player->skills[coSlot], SKILL_RANK_CANNON_GUARD)) {
    reduction += 20;
  }
  return reduction;
}
