#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/skills.h"
#include "game/math.h"

int Unit_GetValuePerHp(
    int playerIndex,
    int unitType); /* unit cost / 10, probably after the CO's cost change */
void Player_AddFunds(int playerIndex, int amount);

/* Skill words of a player's front CO. */
#define FRONT_CO_SKILLS(player) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Pays the attacker's player part of the value of the HP the target lost
 * (hpLost in displayed HP, 0-10): 2% for Combat Pay, 50% for Sasha's
 * Super CO Power. Only the front CO's skills count. */
void DamageToFunds(MapUnit* attacker, MapUnit* target, int hpLost) {
  int attackerOwner =
      MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], attacker);
  int targetOwner =
      MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], target);

  if (CheckSkillsEquipped(FRONT_CO_SKILLS(attackerOwner),
                          SKILL_RANK_COMBAT_PAY)) {
    /* needed to match: the original narrows the player index here */
    Player_AddFunds(
        (u16)attackerOwner,
        HwDiv32(Unit_GetValuePerHp(targetOwner, target->type) * hpLost * 2,
                100));
  }
  if (CheckSkillsEquipped(FRONT_CO_SKILLS(attackerOwner),
                          SKILL_DAMAGE_TO_FUNDS)) {
    Player_AddFunds(
        (u16)attackerOwner,
        HwDiv32(Unit_GetValuePerHp(targetOwner, target->type) * hpLost * 50,
                100));
  }
}
