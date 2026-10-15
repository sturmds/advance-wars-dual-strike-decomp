#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"

int GetUnitCombatClass(int unitType);

/* Movement bonus from the skills of one CO (slot 0 front, 1 back) of a
 * player: +1 for transports with APC Boost. */
int MovementBoost(int player, int slot, int unitType) {
  int move = 0;

  if (GetUnitCombatClass(unitType) == UNIT_CLASS_TRANSPORT &&
      CheckSkillsEquipped(
          data_027c027c->unk_48[data_027c0284]->players[player].skills[slot],
          SKILL_RANK_APC_BOOST)) {
    move += 1;
  }
  return move;
}
