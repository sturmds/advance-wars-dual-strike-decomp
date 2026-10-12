#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern u32 gAiBuildScores
    []; /* build weight per unit type; AiTurn_PickUnitToBuild takes the highest */
extern u16 gAiEnemyHpByType
    []; /* per unit type: total hp of the enemy teams' units (AiBuild_SumUnitHp) */
extern u16 gAiOwnHpByType
    []; /* per unit type: total hp of the current team's units (AiBuild_SumUnitHp) */
extern u8 gAiTargetGroupType
    []; /* per target group: the unit type that stands for it */

int AiBuild_PickAntiAir(void);
int AiBuild_GetTargetGroup(u8 type);
BOOL AiProfile_AllowsBuild(u8 type);
int AiBuild_RateAttackerVsTarget(u8 type, int targetType);
int AiBuild_ScaleByReach(int damage, int type, int arg);
int Ai_BestDamageVsDived(int unitType, int targetType, int powerState);
int Ai_BestDamage(int attacker, int target, int arg);
int Ai_CountUnitsOfType(int type);
int Unit_GetValuePerHp(int playerIndex, int unitType);
s32 HwDiv32(s32 numerator, s32 denominator);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills gAiBuildScores with a build weight for each unit type. It sums the
 * enemy hp per target group, takes off what the current team's units would
 * deal to each group, then weighs every type the AI may build by
 * AiBuild_RateAttackerVsTarget against the groups left. Each unit of that type the player
 * already has cuts the weight by 1/8 (1/4 when no group is left). When the
 * groups are all at 0, types that can hit Infantry get their value. */
void AiBuild_WeighUnitTypes(void) {
  /* needed to match: each unit type loop has its own counter, and the
     * first two loops count with hp and type */
  int i;
  int type;
  int attacker;
  int candidate;
  int option;
  int hitter;
  int group;
  int damage;
  int hp;
  int least;
  int count;
  int groupHp[TARGET_GROUP_COUNT];

  data_021619c0->unk_06 = AiBuild_PickAntiAir();
  for (hp = 0; hp < UNIT_TYPE_COUNT; hp++) {
    gAiBuildScores[hp] = 0;
  }
  for (type = 0; type < TARGET_GROUP_COUNT; type++) {
    groupHp[type] = 0;
  }
  for (type = 1; type < UNIT_TYPE_COUNT; type++) {
    group = AiBuild_GetTargetGroup(type);
    if (group != TARGET_GROUP_NONE) {
      groupHp[group] += gAiEnemyHpByType[type];
    }
  }
  /* Transports count double on a map with ships but no aircraft. */
  if (gAiTurnState->domainsAny & AI_DOMAIN_SEA) {
    if (!(gAiTurnState->domainsAny & AI_DOMAIN_AIR)) {
      groupHp[TARGET_GROUP_TRANSPORT] *= 2;
    }
  }
  for (attacker = 1; attacker < UNIT_TYPE_COUNT; attacker++) {
    if (!AiProfile_AllowsBuild(attacker)) {
      continue;
    }
    for (group = 0; group < TARGET_GROUP_COUNT; group++) {
      if (groupHp[group] == 0) {
        continue;
      }
      if (gAiTargetGroupType[group] == UNIT_SUB) {
        damage = Ai_BestDamageVsDived(attacker, gAiTargetGroupType[group], 0) *
                 gAiOwnHpByType[attacker];
      } else {
        damage = Ai_BestDamage(attacker, gAiTargetGroupType[group], 0) *
                 gAiOwnHpByType[attacker];
      }
      damage = AiBuild_ScaleByReach(damage, attacker, 0);
      if (Ov000_gUnitStats[gAiTargetGroupType[group]].moveGroup &
          MOVE_GROUP_AIR) {
        /* Of Anti-Air and Cruiser, the one AiBuild_PickAntiAir did not pick counts half. */
        if (attacker == UNIT_ANTI_AIR) {
          if (data_021619c0->unk_06 == AI_ANTI_AIR_CRUISER ||
              data_021619c0->unk_06 == AI_ANTI_AIR_CRUISER_MORE) {
            damage >>= 1;
          }
        }
        if (attacker == UNIT_CRUISER) {
          if (data_021619c0->unk_06 == AI_ANTI_AIR_AA ||
              data_021619c0->unk_06 == AI_ANTI_AIR_AA_MORE) {
            damage >>= 1;
          }
        }
      }
      groupHp[group] -= HwDiv32(damage, 100);
    }
  }
  for (i = 0; i < TARGET_GROUP_COUNT; i++) {
    if (groupHp[i] > 0) {
      goto someLeft;
    }
  }
  goto noneLeft;
someLeft:
  for (candidate = 1; candidate < UNIT_TYPE_COUNT; candidate++) {
    if (!AiProfile_AllowsBuild(candidate)) {
      continue;
    }
    if (candidate == UNIT_ANTI_AIR &&
        !(gAiTurnState->domainsAny & AI_DOMAIN_AIR)) {
      continue;
    }
    for (i = 0; i < TARGET_GROUP_COUNT; i++) {
      hp = groupHp[i];
      if (hp > 0) {
        damage = AiBuild_RateAttackerVsTarget(candidate, gAiTargetGroupType[i]);
        gAiBuildScores[candidate] += damage * hp;
      }
    }
    for (count = 0; count < Ai_CountUnitsOfType(candidate); count++) {
      gAiBuildScores[candidate] -= gAiBuildScores[candidate] >> 3;
    }
  }
  return;
noneLeft:
  /* Every group is covered: weigh by how far each one is overkilled,
     * the most overkilled group counting 1. */
  least = 0;
  for (i = 0; i < TARGET_GROUP_COUNT; i++) {
    if (least > groupHp[i]) {
      least = groupHp[i];
    }
  }
  if (least != 0) {
    for (option = 1; option < UNIT_TYPE_COUNT; option++) {
      if (!AiProfile_AllowsBuild(option)) {
        continue;
      }
      if (option == UNIT_ANTI_AIR &&
          !(gAiTurnState->domainsAny & AI_DOMAIN_AIR)) {
        continue;
      }
      for (i = 0; i < TARGET_GROUP_COUNT; i++) {
        hp = groupHp[i];
        if (hp != 0) {
          damage = AiBuild_RateAttackerVsTarget(option, gAiTargetGroupType[i]);
          gAiBuildScores[option] += damage * (hp - (least - 1));
        }
      }
      for (count = 0; count < Ai_CountUnitsOfType(option); count++) {
        gAiBuildScores[option] -= gAiBuildScores[option] >> 2;
      }
    }
    return;
  }
  for (hitter = 1; hitter < UNIT_TYPE_COUNT; hitter++) {
    if (!AiProfile_AllowsBuild(hitter)) {
      continue;
    }
    if (hitter == UNIT_ANTI_AIR &&
        !(gAiTurnState->domainsAny & AI_DOMAIN_AIR)) {
      continue;
    }
    if (Ai_BestDamage(hitter, UNIT_INFANTRY, 0) == 0) {
      continue;
    }
    gAiBuildScores[hitter] = Unit_GetValuePerHp(STATE->currentPlayer, hitter);
    for (count = 0; count < Ai_CountUnitsOfType(hitter); count++) {
      gAiBuildScores[hitter] -= gAiBuildScores[hitter] >> 2;
    }
  }
}
