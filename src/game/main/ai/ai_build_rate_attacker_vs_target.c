#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/unit_stats.h"

int Ai_CountUnitsOfType(int type);
s32 HwDiv32(s32 numerator, s32 denominator);
int Ai_CoPairBestDamageVsDived(int attackerType, int targetType, int arg);
int Ai_CoPairBestDamage(int attackerType, int targetType, int arg);
int Ai_BestDamage(int attackerType, int targetType, int arg);
int AiBuild_ScaleByReach(int score, int unitType, int arg);

#define MOVE_GROUP_GROUND \
  (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)

/* Rates a unit type to build against one enemy unit type, for the AI's
 * build scores. Starts from a damage value (Ai_CoPairBestDamage halved against
 * Infantry, that plus Ai_CoPairBestDamageVsDived against Subs, else Ai_BestDamage),
 * halves it for ground units against ships, doubles it for ships against
 * ships, for Subs against Battleships and against aircraft (twice for
 * Fighters). Anti-Air and Cruisers against aircraft also depend on
 * AiBuildState.unk_06 and on whether the player has one already. The
 * result goes through AiBuild_ScaleByReach and the profile's percentage for the
 * attacker type. */
int AiBuild_RateAttackerVsTarget(int attacker, int target) {
  int score;

  /* Anti-Air against foot units (and UNIT_NONE) is rated as a Tank */
  if (attacker == UNIT_ANTI_AIR && (u32)target <= UNIT_MECH) {
    attacker = UNIT_TANK;
  }
  if (target == UNIT_INFANTRY) {
    score = Ai_CoPairBestDamage(attacker, target, 2) >> 1;
  } else if (target == UNIT_SUB) {
    score = Ai_CoPairBestDamage(attacker, target, 2) >> 1;
    score += Ai_CoPairBestDamageVsDived(attacker, target, 2);
  } else {
    score = Ai_BestDamage(attacker, target, 2);
  }
  if (Ov000_gUnitStats[attacker].moveGroup & MOVE_GROUP_GROUND) {
    if (Ov000_gUnitStats[target].moveGroup & MOVE_GROUP_SEA) {
      score >>= 1;
    }
  }
  if (Ov000_gUnitStats[attacker].moveGroup & MOVE_GROUP_SEA) {
    if (Ov000_gUnitStats[target].moveGroup & MOVE_GROUP_SEA) {
      score <<= 1;
    }
  }
  if (attacker == UNIT_SUB && target == UNIT_BATTLESHIP) {
    score <<= 1;
  }
  if (Ov000_gUnitStats[target].moveGroup & MOVE_GROUP_AIR) {
    score <<= 1;
    if (attacker == UNIT_FIGHTER) {
      score <<= 1;
    }
  }
  if (attacker == UNIT_ANTI_AIR &&
      (Ov000_gUnitStats[target].moveGroup & MOVE_GROUP_AIR)) {
    if (data_021619c0->unk_06 == 2 || data_021619c0->unk_06 == 4) {
      if (Ai_CountUnitsOfType(UNIT_ANTI_AIR) != 0) {
        score >>= 1;
      }
    }
    if (data_021619c0->unk_06 == 3) {
      score <<= 1;
    }
  }
  if (attacker == UNIT_CRUISER &&
      (Ov000_gUnitStats[target].moveGroup & MOVE_GROUP_AIR)) {
    if (data_021619c0->unk_06 == 1 || data_021619c0->unk_06 == 3) {
      if (Ai_CountUnitsOfType(UNIT_CRUISER) != 0) {
        score >>= 1;
      }
    }
    if (data_021619c0->unk_06 == 4) {
      score <<= 1;
    }
  }
  score = AiBuild_ScaleByReach(score, attacker, 2);
  score *= gAiProfile->units[attacker - 1].unk_00;
  return HwDiv32(score, 100);
}
