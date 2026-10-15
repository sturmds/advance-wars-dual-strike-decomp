#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

int GetUnitCombatClass(int unitType);

/* Defense bonus from skills for one CO (slot 0 front, 1 back) of a player:
 * +10 with a Bodyguard tag partner, +10 more for transports with APC Guard. */
int DefenseIncreaseTagTransport(int player, int slot, int unitType) {
  int defense = 0;

  if (CheckSkillsEquipped(
          data_027c027c->unk_48[data_027c0284]->players[player].skills[slot],
          SKILL_PARTNER_BODYGUARD)) {
    defense += 10;
  }
  if (GetUnitCombatClass(unitType) == UNIT_CLASS_TRANSPORT &&
      CheckSkillsEquipped(
          data_027c027c->unk_48[data_027c0284]->players[player].skills[slot],
          SKILL_RANK_APC_GUARD)) {
    defense += 10;
  }
  return defense;
}
