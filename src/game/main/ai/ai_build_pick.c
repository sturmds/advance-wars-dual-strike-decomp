#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/ai_site.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/ai_build_state.h"
#include "game/ai/ai_end_actions.h"
#include "game/range_map.h"
#include "game/ai_profile.h"
#include "game/math.h"
#include "game/co_data.h"
#include "game/ai/ai_build_pick.h"
#include "game/ai/ai_site_pick.h"
#include "game/ai/ai_build_checks.h"
#include "game/ai/ai_best_damage.h"
#include "game/ai/ai_count_units.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define HQ_X (MAP_STATE->players[MAP_STATE->currentPlayer].hqX & 0x7f)
#define HQ_Y (MAP_STATE->players[MAP_STATE->currentPlayer].hqY & 0x7f)
#define BASE_NEAR_HQ_RANGE 4
#define PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])
#define TERRAIN_PIPE 15 /* guess: TERRAIN_PIPE_SEAM is 16 */
#define PIPE_MOVE_LIMIT \
  110 /* RangeMap_FillMoves movement; tiles up to it are reachable */
#define AI_PROFILE_PIPERUNNERS 1
#define AI_PROFILE_BLACK_BOMBS 2
#define SITE_RADIUS (gAiProfile->unk_08[8]) /* AiProfile byte 0x10 */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])
#define MOVE_GROUP_GROUND \
  (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)

extern u32 gAiBuildScores[];
/* per unit type: build score; 0: not a candidate */
extern u8 gUnitSupplyKind[];
extern u16 gAiEnemyHpByType[UNIT_TYPE_COUNT];
/* per unit type: hp of enemy units */
extern u16 gAiOwnHpByType[UNIT_TYPE_COUNT];
/* per unit type: total hp of the current team's units (AiBuild_SumUnitHp) */
extern u8 gAiTargetGroupType[];
extern s8 data_ov000_022b3850[UNIT_TYPE_COUNT];

BOOL Player_IsInGame(u8 player);
BOOL Player_HasHq(int player);
int UnitType_GetAttackKind(u8 unitType);
int Unit_GetValuePerHp(int playerIndex, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(s8 mark);
int UnitType_GetMaxRange(int unitType);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
/* per unit type: hp of the current player's team */
/* The y byte of each AiSite record (data_0229e5bc + 1); the original
 * addresses it through its own symbol. */
extern u8 data_0229e5bd[][sizeof(AiSite)];
BOOL UnitSlot_OwnsTile(int slot, int x, int y);
int CurrentPlayer_GetUnitMaxRange(int unitType, int powerState);
int CurrentPlayer_GetUnitMoveRange(int unitType, int powerState);

/* Target group (TARGET_GROUP_*) of a unit type, for AiBuild_WeighUnitTypes. */
int AiBuild_GetTargetGroup(u8 unitType) {
  switch (unitType) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      return TARGET_GROUP_FOOT;
    case UNIT_RECON:
    case UNIT_APC:
    case UNIT_PIPERUNNER:
    case UNIT_ARTILLERY:
    case UNIT_ROCKETS:
    case UNIT_MISSILES:
      return TARGET_GROUP_LIGHT;
    case UNIT_MEDIUM_TANK:
    case UNIT_TANK:
    case UNIT_NEOTANK:
    case UNIT_ANTI_AIR:
      return TARGET_GROUP_TANK;
    case UNIT_STEALTH:
    case UNIT_BLACK_BOMB:
    case UNIT_FIGHTER:
    case UNIT_BOMBER:
      return TARGET_GROUP_PLANE;
    case UNIT_B_COPTER:
    case UNIT_T_COPTER:
      return TARGET_GROUP_COPTER;
    case UNIT_BATTLESHIP:
      return TARGET_GROUP_BATTLESHIP;
    case UNIT_CRUISER:
      return TARGET_GROUP_CRUISER;
    case UNIT_BLACK_BOAT:
    case UNIT_LANDER:
    case UNIT_CARRIER:
      return TARGET_GROUP_TRANSPORT;
    case UNIT_SUB:
      return TARGET_GROUP_SUB;
    case UNIT_OOZIUM:
    default:
      return TARGET_GROUP_NONE;
  }
}

/* Scales a build score by how far the unit type reaches, in eighths: its
 * max range plus 4 less its min range for indirect units, its movement for
 * the rest (both with the current player's CO bonuses). Recon counts half. */
int AiBuild_ScaleByReach(int score, int unitType, int powerState) {
  if (UnitType_GetAttackKind(unitType) == ATTACK_KIND_INDIRECT) {
    score = score * (CurrentPlayer_GetUnitMaxRange(unitType, powerState) + 4 -
                     Ov000_gUnitStats[unitType].minRange) >>
            3;
  } else {
    score = score * CurrentPlayer_GetUnitMoveRange(unitType, powerState) >> 3;
  }
  if (unitType == UNIT_RECON) {
    score >>= 1;
  }
  return score;
}

/* Rates a unit type to build against one enemy unit type, for the AI's
 * build scores. Starts from a damage value (Ai_CoPairBestDamage halved against
 * Infantry, that plus Ai_CoPairBestDamageVsDived against Subs, else Ai_BestDamage),
 * halves it for ground units against ships, doubles it for ships against
 * ships, for Subs against Battleships and against aircraft (twice for
 * Fighters). Anti-Air and Cruisers against aircraft also depend on
 * AiBuildState.unk_06 and on whether the player has one already. The
 * result goes through AiBuild_ScaleByReach and the profile's percentage for the
 * attacker type. */
int AiBuild_RateAttackerVsTarget(u8 attacker, int target) {
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

/* unit info panel entry per type; -1: none */
/* True when the type has an info panel entry, is not a Piperunner or Oozium
 * (never allowed here), and AiProfile.units[].unk_00 is set for it. */
BOOL AiProfile_AllowsBuild(u8 unitType) {
  if (unitType >= UNIT_TYPE_COUNT) {
    return FALSE;
  }
  if (data_ov000_022b3850[unitType] == -1) {
    return FALSE;
  }
  if (unitType == UNIT_PIPERUNNER) {
    return FALSE;
  }
  if (unitType == UNIT_OOZIUM) {
    return FALSE;
  }
  return gAiProfile->units[unitType - 1].unk_00 != 0;
}

/* Picks Anti-Air or Cruiser (AI_ANTI_AIR_*) to answer enemy aircraft; NONE
 * when the current player has no sea domain. An enemy aircraft on a tile a
 * Cruiser can enter counts for Cruisers, any other for Anti-Air; aircraft on
 * an airport their owner holds do not count. Each Anti-Air or Cruiser of the
 * current team takes one off its side. The _MORE results: enemy planes and
 * B-Copters have more hp than the team's units that can hit planes. */
int AiBuild_PickAntiAir(void) {
  /* needed to match: separate locals for the enemy loop, declared last */
  int aaNeed;
  int cruiserNeed;
  int airHp;
  int player;
  int slot;
  MapUnit* unit;
  int enemySlot;
  MapUnit* enemy;

  aaNeed = 0;
  cruiserNeed = 0;
  airHp = 0;
  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return AI_ANTI_AIR_NONE;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
        MAP_STATE->players[player].team) {
      for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
        unit = &MAP_STATE->units[slot];
        if (unit->type == UNIT_NONE) {
          continue;
        }
        if (unit->type == UNIT_ANTI_AIR) {
          aaNeed--;
        }
        if (unit->type == UNIT_CRUISER) {
          cruiserNeed--;
        }
        if (Ov000_gUnitStats[unit->type].targetMask & UNIT_CATEGORY_PLANE) {
          airHp -= unit->hp;
        }
      }
    } else {
      for (enemySlot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           enemySlot < Ov000_gPlayerFirstUnitSlot[player] + 51; enemySlot++) {
        enemy = &MAP_STATE->units[enemySlot];
        if (enemy->type == UNIT_NONE) {
          continue;
        }
        if (!(Ov000_gUnitStats[enemy->type].moveGroup & MOVE_GROUP_AIR)) {
          continue;
        }
        if (MAP_TILE_TERRAIN(MAP_TILE_AT(enemy->x, enemy->y)) ==
                TERRAIN_AIRPORT &&
            UnitSlot_OwnsTile((s16)enemySlot, enemy->x, enemy->y) == 1) {
          continue;
        }
        if (CHART[Ov000_gUnitStats[UNIT_CRUISER].moveType]
                 [MAP_TILE_TERRAIN(MAP_TILE_AT(enemy->x, enemy->y))] > 0) {
          cruiserNeed++;
        } else {
          aaNeed++;
        }
        if (enemy->type != UNIT_T_COPTER &&
            (Ov000_gUnitStats[enemy->type].categoryMask &
             (UNIT_CATEGORY_PLANE | UNIT_CATEGORY_COPTER))) {
          airHp += enemy->hp;
        }
      }
    }
  }
  if (aaNeed < 0 && cruiserNeed < 0) {
    return AI_ANTI_AIR_NONE;
  }
  if (aaNeed > cruiserNeed) {
    return airHp > 0 ? AI_ANTI_AIR_AA_MORE : AI_ANTI_AIR_AA;
  }
  if (aaNeed < cruiserNeed) {
    return airHp > 0 ? AI_ANTI_AIR_CRUISER_MORE : AI_ANTI_AIR_CRUISER;
  }
  return AI_ANTI_AIR_NONE;
}

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
    gAiBuildScores[hitter] =
        Unit_GetValuePerHp(MAP_STATE->currentPlayer, hitter);
    for (count = 0; count < Ai_CountUnitsOfType(hitter); count++) {
      gAiBuildScores[hitter] -= gAiBuildScores[hitter] >> 2;
    }
  }
}

/* Like AiBuild_SumUnitHp, but only counts units within SITE_RADIUS tiles of
 * a site (Manhattan distance). Ground units count only when a Tank could
 * reach their tile from the site within SITE_RADIUS move points. Then calls
 * AiBuild_WeighUnitTypes. */
void AiSite_SumNearbyUnitHp(int siteIndex) {
  /* needed to match: declaration order, the u8 types and the
     * radius-first sums below */
  int maxX;
  int minY;
  MapUnit* unit;
  int y;
  int dist;
  int maxY;
  int x;
  int type;
  int minX;
  u8 slot;
  int siteX;
  u8 siteY;

  for (type = 0; type < UNIT_TYPE_COUNT; type++) {
    gAiOwnHpByType[type] = gAiEnemyHpByType[type] = 0;
  }
  RangeMap_FillMoves(data_0229e5bc[siteIndex].x, data_0229e5bd[siteIndex][0],
                     UNIT_TANK, SITE_RADIUS, 0);

  siteX = data_0229e5bc[siteIndex].x;
  minX = siteX - SITE_RADIUS;
  if (minX < 0) {
    minX = 0;
  }
  siteY = data_0229e5bd[siteIndex][0];
  minY = siteY - SITE_RADIUS;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (SITE_RADIUS + siteX < maxX) {
    maxX = SITE_RADIUS + siteX;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (SITE_RADIUS + siteY < maxY) {
    maxY = SITE_RADIUS + siteY;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (data_0229e5bc[siteIndex].x - x > 0) {
        dist += data_0229e5bc[siteIndex].x - x;
      } else {
        dist += x - data_0229e5bc[siteIndex].x;
      }
      if (data_0229e5bc[siteIndex].y - y > 0) {
        dist += data_0229e5bc[siteIndex].y - y;
      } else {
        dist += y - data_0229e5bc[siteIndex].y;
      }
      if (dist < 0 || dist > SITE_RADIUS) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      if (slot == 0) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      type = unit->type;
      if ((Ov000_gUnitStats[type].moveGroup &
           (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) &&
          MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE->players[(slot >> 6) + 1].team) {
        gAiOwnHpByType[type] += unit->hp;
      } else {
        gAiEnemyHpByType[type] += unit->hp;
      }
    }
  }
  AiBuild_WeighUnitTypes();
}

/* Adds up unit hp per unit type, once for the current player's team and once
 * for the players on other teams, then calls AiBuild_WeighUnitTypes. Each player
 * owns slots first + 1 to first + 50 of Unk_02080440_48.units. */
void AiBuild_SumUnitHp(void) {
  /* needed to match: each loop has its own counter */
  int type;
  int player;
  int slot;
  int enemySlot;
  MapUnit* unit;

  for (type = 0; type < UNIT_TYPE_COUNT; type++) {
    gAiOwnHpByType[type] = gAiEnemyHpByType[type] = 0;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
        MAP_STATE->players[player].team) {
      for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
        unit = &MAP_STATE->units[slot];
        if (unit->type != UNIT_NONE) {
          gAiOwnHpByType[unit->type] += unit->hp;
        }
      }
    } else {
      for (enemySlot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           enemySlot < Ov000_gPlayerFirstUnitSlot[player] + 51; enemySlot++) {
        unit = &MAP_STATE->units[enemySlot];
        if (unit->type != UNIT_NONE) {
          gAiEnemyHpByType[unit->type] += unit->hp;
        }
      }
    }
  }
  AiBuild_WeighUnitTypes();
}

/* True when the AI profile allows Black Bombs (bit 2 of the byte AiProfile_HasBuildFlag
 * tests), the player has no Black Bomb, and a target is ready: with a
 * unitType, AiSite_PickBlackBombAirport finds a site and the player has a
 * unit of that type; with 0, a site of type 2 exists. */
BOOL AiBuild_WantsBlackBombInstead(int unitType) {
  if (!AiProfile_HasBuildFlag(AI_PROFILE_BLACK_BOMBS)) {
    return FALSE;
  }
  if (unitType != 0) {
    if (AiSite_PickBlackBombAirport() == -1) {
      return FALSE;
    }
  } else if (!AiSites_HasType(SITE_AIRPORT)) {
    return FALSE;
  }
  if (Ai_CountUnitsOfType(UNIT_BLACK_BOMB) != 0) {
    return FALSE;
  }
  if (unitType != 0 && Ai_CountUnitsOfType(unitType) == 0) {
    return FALSE;
  }
  return TRUE;
}

/* FALSE when the current player cannot pay for a Black Bomb; otherwise
 * whatever AiBuild_WantsBlackBombInstead says (it also refuses while an own Black Bomb exists). */
BOOL Ai_WantsBlackBomb(int arg) {
  if ((u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_BLACK_BOMB) *
            10) > MAP_STATE->players[MAP_STATE->currentPlayer].funds) {
    return FALSE;
  }
  return AiBuild_WantsBlackBombInstead(arg);
}

/* True when the AI profile allows Piperunners (bit 1 of the byte AiProfile_HasBuildFlag
 * tests), a site wants one, and the player has more units of unitType than
 * Piperunners. */
BOOL AiBuild_WantsPiperunnerInstead(int unitType) {
  if (!AiProfile_HasBuildFlag(AI_PROFILE_PIPERUNNERS)) {
    return FALSE;
  }
  if (AiSite_PickPiperunnerSite() == -1) {
    return FALSE;
  }
  return Ai_CountUnitsOfType(unitType) > Ai_CountUnitsOfType(UNIT_PIPERUNNER);
}

/* AiBuild_WantsPiperunnerInstead, but only when the player has the funds for
 * a Piperunner. */
BOOL AiBuild_CanAffordPiperunnerInstead(int unitType) {
  if (Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_PIPERUNNER) * 10 >
      (u32)PLAYER.funds) {
    return FALSE;
  }
  return AiBuild_WantsPiperunnerInstead(unitType);
}

/* End-actions step: rates each base AiSite next to a pipe tile for
 * Piperunners. It fills the Piperunner move and attack range from the site,
 * then counts the units on it: the current player's Piperunners within move
 * range, and armed enemy units (4 for a Piperunner, 1 for any other).
 * score = (enemies + 4) / 8 - own Piperunners. */
void AiSite_ScorePiperunnerNeed(void) {
  /* needed to match: declaration order sets the register choice, and the
     * second scan uses its own row and col */
  int maxY;
  int minX;
  MapUnit* unit;
  AiSite* site;
  int maxX;
  int slot;
  int y;
  int dist;
  int minY;
  u8 found;
  int x;
  int ownCount;
  int i;
  int range;
  int enemyCount;
  int col;
  int row;

  site = data_0229e5bc;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind == SITE_KIND_SKIPPED || site->type != SITE_BASE) {
      continue;
    }
    found = 0;
    minX = site->x - 1;
    if (minX < 0) {
      minX = 0;
    }
    minY = site->y - 1;
    if (minY < 0) {
      minY = 0;
    }
    maxX = MAP_STATE->unk_1c4->mapWidth - 1;
    if (site->x + 1 < maxX) {
      maxX = site->x + 1;
    }
    maxY = MAP_STATE->unk_1c4->mapHeight - 1;
    if (site->y + 1 < maxY) {
      maxY = site->y + 1;
    }
    for (y = minY; y <= maxY; y++) {
      for (x = minX; x <= maxX; x++) {
        dist = 0;
        if (site->x - x > 0) {
          dist += site->x - x;
        } else {
          dist += x - site->x;
        }
        if (site->y - y > 0) {
          dist += site->y - y;
        } else {
          dist += y - site->y;
        }
        if (dist < 1 || dist > 1) {
          continue;
        }
        if ((u32)(MAP_TILE_TERRAIN(
                      MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) -
                  TERRAIN_PIPE) <= 1) { /* pipe or pipe seam */
          found = 1;
        }
      }
    }
    if (!found) {
      continue;
    }

    RangeMap_FillMoves(site->x, site->y, UNIT_PIPERUNNER, PIPE_MOVE_LIMIT, 1);
    for (range = 0; range < UnitType_GetMaxRange(UNIT_PIPERUNNER); range++) {
      RangeMap_MarkAdjacentTiles(PIPE_MOVE_LIMIT + 1 + range);
    }

    ownCount = 0;
    enemyCount = 0;
    for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
      for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
        if (MAP_STATE->rangeMap[row][col] < 0) {
          continue;
        }
        slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(col, row)];
        if (slot == 0) {
          continue;
        }
        if (Unit_IsCurrentPlayers(slot) == 1) {
          if (MAP_STATE->rangeMap[row][col] > PIPE_MOVE_LIMIT) {
            continue;
          }
          if (MAP_STATE->units[slot].type == UNIT_PIPERUNNER) {
            ownCount++;
          }
        } else {
          if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
              MAP_STATE->players[(slot >> 6) + 1].team) {
            continue;
          }
          unit = &MAP_STATE->units[slot];
          if (UnitType_GetMaxRange(unit->type) != 0) {
            if (unit->type == UNIT_PIPERUNNER) {
              enemyCount += 4;
            } else {
              enemyCount += 1;
            }
          }
        }
      }
    }
    site->score = ((enemyCount + 4) >> 3) - ownCount;
  }
}

/* Called by func_02091904 (end-actions step 9). Picks the unit type with the highest weight in
 * gAiBuildScores and a factory for it (AiSite_PickForBuild by gUnitSupplyKind).
 * AiSite_RateBaseLandmass returns flags (bit 8) and a count (bits 0-7) that rule out
 * foot units or, for ground units, all but non-direct ones, Infantry, Mech and
 * Anti-Air (at half weight). An airport or port where AiBuild_RateSiteDanger gives 90 or
 * more is marked (kind 0xff) and the next one tried. When funds are short and
 * allowSaving is 1, AiBuild_ShouldSaveFunds may set AiBuildState.savingTarget to the funds
 * still missing after next income; while savingTarget is set, a build must leave
 * that much (savingSkips times it may skip the check for a type the player has
 * none of). Black Bomb and Piperunner builds go through AiSite_PickBlackBombAirport and
 * AiSite_PickPiperunnerSite. A type that fails gets weight 0 and the pick starts again.
 * Returns the factory's site index with gUnitAction.target set, or -1. */
int AiTurn_PickUnitToBuild(int allowSaving) {
  /* needed to match: this declaration order and the hoisted countLess1 */
  int index;
  u8* supplyKind;
  int best;
  int infoFlag;
  u32 bestScore;
  int pipeCost;
  int info;
  int type;
  UnitStats* stats;
  int countLess1;
  AiSite* site;
  int infoCount;
  u16 cost;

  if (AiSite_PickFirst(4) == -1) {
    return -1;
  }
  info = AiSite_RateBaseLandmass();
  infoFlag = info & 0x100;
  infoCount = info & 0xff;
  for (;;) {
    best = 0;
    bestScore = 0;
    countLess1 = infoCount - 1;
    for (type = 1, stats = &Ov000_gUnitStats[1]; type < UNIT_TYPE_COUNT;
         stats++, type++) {
      if (bestScore >= gAiBuildScores[type]) {
        continue;
      }
      if (infoFlag != 0) {
        if (type <= UNIT_MECH && countLess1 >= 5) {
          continue;
        }
      } else if (infoCount != 0 &&
                 (stats->moveGroup &
                  (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
        if (UnitType_GetAttackKind(type) != ATTACK_KIND_DIRECT ||
            type == UNIT_INFANTRY || type == UNIT_MECH ||
            type == UNIT_ANTI_AIR) {
          if (type == UNIT_ARTILLERY) {
            continue;
          }
          if (type <= UNIT_MECH && countLess1 >= 5) {
            continue;
          }
          if (bestScore <= gAiBuildScores[type] >> 1) {
            bestScore = gAiBuildScores[type] >> 1;
            best = type;
          }
        }
        continue;
      }
      if (type == UNIT_ARTILLERY) {
        if (AiBuild_HasEnoughIndirectsForArtillery(info) == 1) {
          continue;
        }
      } else if (UnitType_GetAttackKind(type) == ATTACK_KIND_INDIRECT &&
                 (stats->moveGroup &
                  (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) &&
                 AiBuild_HasEnoughGroundIndirects(info) == 1) {
        continue;
      }
      best = type;
      bestScore = gAiBuildScores[type];
    }
    if (best == 0) {
      return -1;
    }
    supplyKind = &gUnitSupplyKind[best];
    for (;;) {
      index = AiSite_PickForBuild(*supplyKind);
      if (index == -1) {
        goto reject;
      }
      /* needed to match: airport or port written as + 254 */
      if ((u8)(*supplyKind + 254) > 1) {
        break;
      }
      site = &data_0229e5bc[index];
      if (AiBuild_RateSiteDanger(site->x, site->y, best) >= 90) {
        site->kind = 0xff;
        continue;
      }
      AiSites_ClearRejectedMarks();
      break;
    }
    cost = Unit_GetValuePerHp(MAP_STATE->currentPlayer, best) * 10;
    if (cost > (u32)PLAYER.funds) {
      if (allowSaving == 1 && AiBuild_ShouldSaveFunds(cost) == 1) {
        data_021619c0->savingTarget =
            cost - (PLAYER.unk_08 - data_021619c0->unk_0c);
      }
      goto reject;
    }
    if (Ai_WantsBlackBomb(best) == 1) {
      gUnitAction.target = UNIT_BLACK_BOMB;
      return AiSite_PickBlackBombAirport();
    }
    if (data_021619c0->savingTarget != 0) {
      if (AiBuild_CanAffordPiperunnerInstead(best) == 1) {
        gUnitAction.target = UNIT_PIPERUNNER;
        return AiSite_PickPiperunnerSite();
      }
    } else if (AiBuild_WantsPiperunnerInstead(best) == 1) {
      pipeCost =
          Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_PIPERUNNER) * 10;
      if (pipeCost > (u32)PLAYER.funds) {
        if (allowSaving == 1 && AiBuild_ShouldSaveFunds(pipeCost) == 1) {
          data_021619c0->savingTarget =
              pipeCost - (PLAYER.unk_08 - data_021619c0->unk_0c);
          continue;
        }
      } else {
        gUnitAction.target = UNIT_PIPERUNNER;
        return AiSite_PickPiperunnerSite();
      }
    }
    if (data_021619c0->savingTarget != 0 && data_021619c0->savingSkips != 0) {
      data_021619c0->savingSkips--;
      if (Ai_CountUnitsOfType(best) != 0 &&
          cost > PLAYER.funds - data_021619c0->savingTarget) {
        goto reject;
      }
    } else if (cost > PLAYER.funds - data_021619c0->savingTarget) {
      goto reject;
    }
    gUnitAction.target = best;
    return index;
  reject:
    gAiBuildScores[best] = 0;
    AiSites_ClearRejectedMarks();
  }
}

/* Picks the unit type to build at a site: the type with the highest score
 * in gAiBuildScores, skipping ground indirect units, whose supply kind matches
 * the site type (base, airport or port).
 * A type the player cannot afford, or that fails, gets score 0 and the search
 * starts again. A type passes when AiBuild_RateSiteDanger rates it below 90; a ground
 * or sea type rated 90 or more gets one more pass that only asks for its cost
 * to be at most half the income or half the funds. Stores the type in
 * gUnitAction.target and returns siteIndex; -1 when nothing is left. */
int AiSite_ChooseUnitToBuild(int siteIndex) {
  /* needed to match: declaration order sets the registers */
  Player* player;
  AiSite* site;
  int bestType;
  u32 bestScore;
  int retried;
  u32 cost;
  int type;

  site = &data_0229e5bc[siteIndex];
  retried = 0;
  /* a port, while some player or tile has air units or airports */
  if (site->type == SITE_PORT && (gAiTurnState->domainsAny & AI_DOMAIN_AIR)) {
    return -1;
  }
  for (;;) {
    bestType = 0;
    bestScore = 0;
    for (type = 1; type < UNIT_TYPE_COUNT; type++) {
      if (bestScore >= gAiBuildScores[type]) {
        continue;
      }
      /* needed to match: the (u8) cast */
      if (UnitType_GetAttackKind((u8)type) == ATTACK_KIND_INDIRECT &&
          (Ov000_gUnitStats[type].moveGroup &
           (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
        continue;
      }
      bestScore = gAiBuildScores[type];
      bestType = type;
    }
    if (bestType == 0) {
      return -1;
    }
    if (site->type != gUnitSupplyKind[bestType]) {
      goto reject;
    }
    cost = (u16)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, bestType) * 10);
    player = &MAP_STATE->players[MAP_STATE->currentPlayer];
    if (cost > player->funds) {
      goto reject;
    }
    if (!retried || (Ov000_gUnitStats[bestType].moveGroup & MOVE_GROUP_AIR)) {
      if (AiBuild_RateSiteDanger(site->x, site->y, bestType) < 90) {
        goto found;
      }
      if (Ov000_gUnitStats[bestType].moveGroup & MOVE_GROUP_AIR) {
        goto reject;
      }
      retried = 1;
      continue;
    }
    if (cost > player->unk_08 >> 1 && cost > (u32)player->funds >> 1) {
      goto reject;
    }
  found:
    gUnitAction.target = bestType;
    return siteIndex;
  reject:
    gAiBuildScores[bestType] = 0;
  }
}

/* TRUE when the current player has an HQ and a base of owner unk_04 lies
 * within BASE_NEAR_HQ_RANGE tiles of it (Manhattan distance). Same body as
 * Ai_HasOwnBaseWithin, with the HQ read again at each use.
 * Needed to match: hqRight and hqBottom are computed before the map width
 * and height loads (hqRight found by tools/permuter.py). */
BOOL Ai_HasOwnBaseNearHq(void) {
  int maxY;
  int maxX;
  int y;
  int minY;
  int minX;
  int x;
  int dist;
  int hqRight;
  int hqY;
  int hqBottom;

  if (!Player_HasHq(MAP_STATE->currentPlayer)) {
    return FALSE;
  }
  minX = HQ_X - BASE_NEAR_HQ_RANGE;
  if (minX < 0) {
    minX = 0;
  }
  hqY = HQ_Y;
  minY = hqY - BASE_NEAR_HQ_RANGE;
  if (minY < 0) {
    minY = 0;
  }
  hqRight = HQ_X + BASE_NEAR_HQ_RANGE;
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (hqRight < maxX) {
    maxX = hqRight;
  }
  hqBottom = hqY + BASE_NEAR_HQ_RANGE;
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (hqBottom < maxY) {
    maxY = hqBottom;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (HQ_X - x > 0) {
        dist += HQ_X - x;
      } else {
        dist += x - HQ_X;
      }
      if (HQ_Y - y > 0) {
        dist += HQ_Y - y;
      } else {
        dist += y - HQ_Y;
      }
      if (dist < 0 || dist > BASE_NEAR_HQ_RANGE) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) ==
              TERRAIN_BASE &&
          MAP_STATE->unk_04 ==
              (MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)] & 0xe0)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* TRUE when a player on another team still in the game has an air unit.
 * Each scan skips the player's first unit slot and covers the next 50. */
BOOL Ai_EnemyHasAirUnit(void) {
  int player;
  int i;
  MapUnit* unit;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
        MAP_STATE->players[player].team) {
      continue;
    }
    for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
         i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
      unit = &MAP_STATE->units[i];
      if (unit->type != UNIT_NONE &&
          (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* TRUE when the current player has a Fighter with at least half hp (hp 100 is
 * full). The scan skips the player's first unit slot and covers the next 50.
 * The Fighter builder does not build another one while this holds. */
BOOL Ai_HasHealthyFighter(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type == UNIT_FIGHTER &&
        unit->hp >= 50) {
      return TRUE;
    }
  }
  return FALSE;
}
