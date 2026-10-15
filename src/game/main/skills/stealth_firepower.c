#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

/* Firepower added by Backstab for a dived attacker. Unit_GetChartDamage passes
 * bit 0x20 of the attacker's flags (probably MAP_UNIT_DIVED) as `dived`.
 * Only the front CO's skills count. */
int StealthFirepower(int playerIndex, BOOL dived) {
  int firepower = 0;

  if (dived &&
      CheckSkillsEquipped(
          data_027c027c->unk_48[data_027c0284]->players[playerIndex].skills[0],
          SKILL_RANK_BACKSTAB)) {
    firepower += 15;
  }
  return firepower;
}
