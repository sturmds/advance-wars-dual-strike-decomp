#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_tile_search.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/ai_profile.h"
#include "game/strike_target.h"
#include "game/range_map.h"
#include "game/ai/ai_find_target.h"
#include "game/ai/ai_service.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_move_search.h"
#include "game/ai.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
/* MapUnit.serviceWanted value that AiUnit_SeekSupply and func_020af5f8 act on;
 * this routine clears it once it repairs the unit. */
#define WANTS_SUPPLY 1
#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff
#define CARRY_TABLE(unitType) (Ov000_gUnitStats[unitType].carryTypes)
#define FAR_AWAY \
  110 /* movement of the fill from the site, and the distance of tiles it misses */

extern u8 gTerrainFactoryKind[];

void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL Tile_HasUnitToRepair(s16 x, s16 y);
BOOL Tile_HasUnitToSupply(s16 x, s16 y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Unit_CanRepairAt(MapUnit* unit, int x, int y);
int UnitType_GetAttackKind(u8 unitType);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiAttack_Score(int arg, s16* pos);
void func_020b81a8(void);

/* UnitStats.moveGroup and UnitStats.fuel of each unit type, one row of
 * sizeof(UnitStats) bytes per type. Needed to match: the original reaches
 * them through their own symbols, not through Ov000_gUnitStats. */
/* Whether the acting unit should go for supplies or repairs: it is under
 * 91 hp, or it is an air or sea unit with under half its fuel. */
BOOL AiUnit_NeedsService(void) {
  if (MAP_STATE->actingUnit->hp <= 90) {
    return TRUE;
  }
  if (!(Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup & 7) &&
      MAP_STATE->actingUnit->fuel <
          Ov000_gUnitStats[MAP_STATE->actingUnit->type].fuel >> 1) {
    return TRUE;
  }
  return FALSE;
}

/* Like AiUnit_RetreatToRepair, for a unit that AiUnit_NeedsService says needs repair
 * (guess). AiUnit_PickOrderSite picks a site (the unit's own tile when it finds
 * none); MapTiles.rangeScratch gets the distance from there. The unit moves to
 * the reachable free tile with the lowest threat where Unit_CanRepairAt or
 * Unit_IsBlackHoleOnFlaggedTile holds, ties to the shorter distance (110 when the fill
 * missed the tile). */
void AiUnit_RetreatTowardSite(void) {
  int bestX;
  int bestY;
  int bestThreat;
  int bestDistance;
  int threat;
  int x;
  int y;
  int distance;
  int tile;
  s16 site[2];

  bestX = -1;
  bestY = bestX;
  bestThreat = 0x7fff;
  bestDistance = bestThreat;
  if (!AiUnit_NeedsService()) {
    return;
  }
  func_020b81a8();
  if (!AiUnit_PickOrderSite(site)) {
    site[0] = MAP_STATE->actingUnit->x;
    site[1] = MAP_STATE->actingUnit->y;
  }
  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillMoves(site[0], site[1], MAP_STATE->actingUnit->type, FAR_AWAY,
                     0);
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Unit_CanRepairAt(MAP_STATE->actingUnit, x, y)) {
        if (!Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y)) {
          continue;
        }
      }
      if (MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (Ai_IsTileNeededByAlly(x, y) == 1) {
        continue;
      }
      if (!Ai_CanEndMoveAt(x, y)) {
        continue;
      }
      tile = MAP_TILE_INDEX(x, y);
      threat = MAP_TILES->threatMax[tile] + MAP_TILES->threatSum[tile] * 2;
      distance = MAP_TILES->rangeScratch[tile];
      if (distance < 0) {
        distance = FAR_AWAY;
      }
      if (threat > bestThreat) {
        continue;
      }
      if (threat == bestThreat && distance > bestDistance) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestThreat = threat;
      bestDistance = distance;
    }
  }
  if (bestX == -1) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(bestX, bestY, 2, 0, 0);
}

/* Runs when AiUnit_NeedsService says the acting unit needs repair (guess from its
 * code: hp 90 or less, or low fuel). Like AiUnit_MoveToSafestTile, it moves the
 * unit to the free reachable tile with the lowest threat (MapTiles.threatMax
 * + 2 * threatSum), ties to the lower rangeMap value, but only to tiles where
 * Unit_CanRepairAt holds or Unit_IsBlackHoleOnFlaggedTile does. With AI_STEP_UNIT_AT_TARGET
 * set it ignores tiles with a negative MapTiles.rangeScratch and sets AI_STEP_STAY
 * instead of moving when the best tile is where the unit already stands. */
void AiUnit_RetreatToRepair(void) {
  int bestX;
  int bestY;
  int bestThreat;
  int bestReach;
  int threat;
  int x;
  int y;
  int reach;
  int tile;

  bestX = -1;
  bestY = bestX;
  bestThreat = 0x7fff;
  bestReach = bestThreat;
  if (!AiUnit_NeedsService()) {
    return;
  }
  func_020b81a8();
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) {
        if (MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(x, y)] < 0) {
          MAP_STATE->rangeMap[y][x] = -1;
          continue;
        }
      }
      if (!Unit_CanRepairAt(MAP_STATE->actingUnit, x, y)) {
        if (!Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y)) {
          continue;
        }
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (Ai_IsTileNeededByAlly(x, y) == 1) {
        continue;
      }
      if (!Ai_CanEndMoveAt(x, y)) {
        continue;
      }
      tile = MAP_TILE_INDEX(x, y);
      threat = MAP_STATE->mapTiles->threatMax[tile] +
               MAP_STATE->mapTiles->threatSum[tile] * 2;
      reach = MAP_STATE->rangeMap[y][x];
      if (threat > bestThreat) {
        continue;
      }
      if (threat == bestThreat && reach > bestReach) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestThreat = threat;
      bestReach = reach;
    }
  }
  if (bestX == -1) {
    return;
  }
  if ((gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) &&
      bestX == MAP_STATE->actingUnit->x && bestY == MAP_STATE->actingUnit->y) {
    gAiTurnState->stepFlags |= AI_STEP_STAY;
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(bestX, bestY, 2, 0, 0);
}

/* Picks the free silo the acting unit reaches with the highest rangeMap
 * value, and launches it at the site Strike_FindBestSite finds. Does
 * nothing when no silo is in reach or no site passes the funds check. */
void AiUnit_LaunchSilo(void) {
  int siloX;
  int siloY;
  int bestReach;
  s16 site[2];
  int x;
  int y;
  int reach;

  siloY = -1;
  siloX = siloY;
  bestReach = siloY;
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) !=
          TERRAIN_SILO) {
        continue;
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      reach = MAP_STATE->rangeMap[y][x];
      if (reach > bestReach) {
        siloX = x;
        siloY = y;
        bestReach = reach;
      }
    }
  }
  if (bestReach == -1) {
    return;
  }
  site[0] = siloX;
  site[1] = siloY;
  if (Strike_FindBestSite(site, MAP_STATE->currentPlayer, 1,
                          STRIKE_MODE_AI_FUNDS) != 1) {
    return;
  }
  AiUnit_PerformAction(siloX, siloY, AI_ACTION_LAUNCH_SILO, site[0], site[1]);
}

/* First step of AiUnit_HeadForRepairTile. A direct-attack unit looks for an
 * enemy unit it can hit from a tile in reach, scored by AiAttack_Score with
 * AI_ATTACK_HEAVY_HIT. As in AiFoot_PickAttackTarget, when
 * AiUnit_TeammateOutdamages holds for the target it needs two or more open
 * sides. If it finds one it attacks and gives up its repair request. */
void AiUnit_TryAttackBeforeRepair(void) {
  int maxY;
  int minX;
  int attackX;
  int attackY;
  int bestScore;
  int score;
  int bestSlot;
  int x;
  int y;
  MapUnit* target;
  MapUnit* neighbour;
  s16 pos[2];
  int maxX;
  int minY;
  int openSides;
  int xx;
  int yy;
  int dist;

  attackX = -1;
  bestScore = 0;
  attackY = attackX;
  bestSlot = bestScore;
  if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) != 0) {
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      target =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      if (target->type == UNIT_NONE) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, target)].team) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      score =
          AiAttack_Score(AI_ATTACK_HEAVY_HIT |
                             MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)],
                         pos);
      if (score == 0) {
        continue;
      }
      if (score < bestScore) {
        continue;
      }
      if (AiUnit_TeammateOutdamages(target) == 1) {
        openSides = 0;
        minX = target->x - 1;
        if (minX < 0) {
          minX = 0;
        }
        minY = target->y - 1;
        if (minY < 0) {
          minY = 0;
        }
        maxX = MAP_STATE->unk_1c4->mapWidth - 1;
        if (target->x + 1 < maxX) {
          maxX = target->x + 1;
        }
        maxY = MAP_STATE->unk_1c4->mapHeight - 1;
        if (target->y + 1 < maxY) {
          maxY = target->y + 1;
        }
        for (yy = minY; yy <= maxY; yy++) {
          for (xx = minX; xx <= maxX; xx++) {
            dist = 0;
            if (target->x - xx > 0) {
              dist += target->x - xx;
            } else {
              dist += xx - target->x;
            }
            if (target->y - yy > 0) {
              dist += target->y - yy;
            } else {
              dist += yy - target->y;
            }
            if (dist < 1 || dist > 1) {
              continue;
            }
            if (MAP_STATE->rangeMap[yy][xx] < 0) {
              continue;
            }
            neighbour = &MAP_STATE->units[MAP_STATE->mapTiles
                                              ->unitAt[MAP_TILE_INDEX(xx, yy)]];
            if (neighbour->type != UNIT_NONE &&
                (neighbour->flags & MAP_UNIT_WAITED)) {
              continue;
            }
            openSides++;
          }
        }
        if (openSides <= 1) {
          continue;
        }
      }
      attackX = pos[0];
      attackY = pos[1];
      bestSlot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      bestScore = score;
    }
  }
  if (bestScore == 0) {
    return;
  }
  AI_TARGET_FLAGS->unk_1 = 0;
  MAP_STATE->actingUnit->serviceWanted = 0; /* no longer asks for repairs */
  AiUnit_PerformAction(attackX, attackY, AI_ACTION_ATTACK_UNIT, bestSlot, 0);
}

/* A unit at 50 hp or less that carries nothing joins a unit of its own type
 * it can reach: not carried or carrying, at most 90 hp, hp sum at most 100,
 * and Unit_IsBlackHoleOnFlaggedTile says no. Picks the one with the highest
 * hp, the last on ties. Infantry and mechs try AiUnit_LaunchSilo first. */
void AiUnit_JoinDamagedUnit(void) {
  int bestX;
  int bestY;
  int bestHp;
  int slot;
  MapUnit* unit;

  bestX = -1;
  bestY = bestX;
  bestHp = 0;
  if (MAP_STATE->actingUnit->hp > 50) {
    return;
  }
  if (MAP_STATE->actingUnit->cargo[0] != 0) {
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type == UNIT_NONE) {
      continue;
    }
    if (MAP_STATE->rangeMap[unit->y][unit->x] < 0) {
      continue;
    }
    if (MAP_STATE->actingUnit->type != unit->type) {
      continue;
    }
    if (MAP_STATE->actingUnit == unit) {
      continue;
    }
    if (unit->hp >= 91) {
      continue;
    }
    if (Unit_IsBlackHoleOnFlaggedTile(unit, unit->x, unit->y) == 1) {
      continue;
    }
    if (MAP_STATE->actingUnit->hp + unit->hp > 100) {
      continue;
    }
    if (unit->cargo[0] != 0) {
      continue;
    }
    if (unit->flags & MAP_UNIT_CARRIED) {
      continue;
    }
    if (unit->hp >= bestHp) {
      bestX = unit->x;
      bestY = unit->y;
      bestHp = unit->hp;
    }
  }
  if (bestX == -1) {
    return;
  }
  if (MAP_STATE->actingUnit->type <= UNIT_MECH) {
    AiUnit_LaunchSilo();
  }
  MAP_STATE->actingUnit->serviceWanted = 0;
  AiUnit_PerformAction(bestX, bestY, AI_ACTION_JOIN, 0, 0);
}

/* Picks a free tile where Unit_CanRepairAt or Unit_IsBlackHoleOnFlaggedTile holds and moves
 * the acting unit there, or toward it when the tile is out of reach this
 * turn. The score starts as the rangeMap value, plus the unit's moves on base,
 * airport and port tiles, and 0 on tiles Unit_IsBlackHoleOnFlaggedTile accepts. Tiles in reach
 * whose threat is above the unit profile's limits are skipped. The lowest score
 * is picked, the last on ties. gAiTurnState->unk_05 holds the unit's moves this turn
 * (see AiTurn_RunQueue). The name is a guess. */
void AiUnit_HeadForRepairTile(void) {
  int bestX;
  int bestY;
  int bestScore;
  int score;
  int x;
  int y;
  int reach;
  int tile;

  AiUnit_TryAttackBeforeRepair();
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, 110, 1);
  bestY = -1;
  bestX = bestY;
  bestScore = 0x7fff;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Unit_CanRepairAt(MAP_STATE->actingUnit, x, y)) {
        if (!Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y)) {
          continue;
        }
      }
      if (MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (!AiUnit_HasFuelForTile(x, y, gAiTurnState->unk_05)) {
        continue;
      }
      reach = MAP_STATE->rangeMap[y][x];
      score = reach;
      if (gTerrainFactoryKind[MAP_TILE_TERRAIN(
              MAP_TILES->tiles[MAP_TILE_INDEX(x, y)])] != 0) {
        score = reach + gAiTurnState->unk_05;
      }
      if (reach <= gAiTurnState->unk_05) {
        if (Ai_IsTileNeededByAlly(x, y) == 1) {
          continue;
        }
        tile = MAP_TILE_INDEX(x, y);
        if (MAP_TILES->threatMax[tile] > gAiActiveUnitProfile->threatMaxLimit) {
          continue;
        }
        if (MAP_TILES->threatSum[tile] > gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
        if (Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y) == 1) {
          score = 0;
        }
      }
      if (score <= bestScore) {
        bestX = x;
        bestY = y;
        bestScore = score;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  Ai_ClearActingUnitTarget();
  if (MAP_STATE->rangeMap[bestY][bestX] <= gAiTurnState->unk_05) {
    AiUnit_PerformAction(bestX, bestY, 2, 0, 0);
    return;
  }
  if (Unit_CanRepairAt(MAP_STATE->actingUnit, MAP_STATE->actingUnit->x,
                       MAP_STATE->actingUnit->y) == 1 &&
      (gAiTurnState->stepFlags & AI_STEP_NO_ADJUST)) {
    AiUnit_PerformAction(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y, 2,
                         0, 0);
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}

/* Run when the acting unit's serviceWanted is 1, the value that AiClass3_SupplyUnit
 * and AiBlackBoat_RepairUnit look for. Moves the unit to where it is
 * supplied or repaired. A waited APC in reach: the unit moves next to it and
 * waits. A Cruiser or Carrier in reach (rangeMap value up to gAiTurnState->unk_05)
 * that takes this unit type aboard and has its second cargo slot free: action 7
 * on its tile. Otherwise it scores empty tiles where it is repaired
 * (Unit_CanRepairAt or Unit_IsBlackHoleOnFlaggedTile; a tile for Unit_IsBlackHoleOnFlaggedTile scores 0) by
 * rangeMap value, plus unk_05 on a base, airport or port, and heads for the
 * lowest, waiting there when it can reach it. needed to match: the Cruiser
 * and Carrier cases are written out twice. */
void AiUnit_GoToSupply(void) {
  int bestX = -1;
  u32 bestY = -1; /* needed to match: found by tools/permute.py */
  int bestScore = NO_TILE;
  int slot;
  MapUnit* unit;
  int score;
  int x;
  int y;
  s16 pos[2];

  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      slot = MAP_TILES->unitAt[MAP_TILES->rowStart[y] + x];
      if (slot != 0) {
        unit = &MAP_STATE->units[slot];
        switch (unit->type) {
          case UNIT_APC:
            if ((unit->flags & MAP_UNIT_WAITED) && FindAttackTile(x, y, pos)) {
              AiUnit_PerformAction(pos[0], pos[1], AI_ACTION_WAIT, 0, 0);
            }
            break;
          case UNIT_CRUISER:
            if (MAP_STATE->rangeMap[y][x] <= gAiTurnState->unk_05 &&
                (CARRY_TABLE(unit->type) + MAP_STATE->actingUnit->type)[1] !=
                    0 &&
                unit->cargo[1] == 0) {
              AiUnit_PerformAction(x, y, AI_ACTION_BOARD, 0, 0);
            }
            break;
          case UNIT_CARRIER:
            if (MAP_STATE->rangeMap[y][x] <= gAiTurnState->unk_05 &&
                (CARRY_TABLE(unit->type) + MAP_STATE->actingUnit->type)[1] !=
                    0 &&
                unit->cargo[1] == 0) {
              AiUnit_PerformAction(x, y, AI_ACTION_BOARD, 0, 0);
            }
            break;
        }
        continue;
      }
      if (!Unit_CanRepairAt(MAP_STATE->actingUnit, x, y) &&
          !Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y)) {
        continue;
      }
      if (!AiUnit_HasFuelForTile(x, y, gAiTurnState->unk_05)) {
        continue;
      }
      score = MAP_STATE->rangeMap[y][x];
      if (gTerrainFactoryKind[MAP_TILE_TERRAIN(
              MAP_TILES->tiles[MAP_TILE_INDEX(x, y)])] != 0) {
        score += gAiTurnState->unk_05;
      }
      if (Ai_IsTileNeededByAlly(x, y) == 1) {
        continue;
      }
      if (Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y) == 1) {
        score = 0;
      }
      if (score <= bestScore) {
        bestScore = score;
        bestX = x;
        bestY = y;
      }
    }
  }
  if (bestScore == NO_TILE) {
    return;
  }
  Ai_ClearActingUnitTarget();
  if (MAP_STATE->rangeMap[bestY][bestX] <= gAiTurnState->unk_05) {
    AiUnit_PerformAction(bestX, bestY, AI_ACTION_WAIT, 0, 0);
  } else {
    AiUnit_MoveTowardTile(bestX, bestY);
  }
}

/* AI step shared by AiFoot_MoveAndAttack and many queue routines. When the
 * acting unit's serviceWanted is 1 or 2, runs AiUnit_GoToSupply or AiUnit_HeadForRepairTile;
 * infantry and mechs first get AiUnit_LaunchSilo. */
void func_020af5f8(void) {
  if (!Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit,
                                     MAP_STATE->actingUnit->x,
                                     MAP_STATE->actingUnit->y)) {
    AiUnit_JoinDamagedUnit();
  }
  if (MAP_STATE->actingUnit->serviceWanted == 0) {
    return;
  }
  if (MAP_STATE->actingUnit->type <= UNIT_MECH) {
    AiUnit_LaunchSilo();
  }
  gAiTurnState->flags |= 2;
  AiStep_DebugHook();
  switch (MAP_STATE->actingUnit->serviceWanted) {
    case 1:
      AiUnit_GoToSupply();
      break;
    case 2:
      AiUnit_HeadForRepairTile();
      break;
  }
}

/* Like func_020af5f8, but only runs AiUnit_GoToSupply and only when the acting
 * unit's serviceWanted is 1. */
void AiUnit_SeekSupply(void) {
  if (!Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit,
                                     MAP_STATE->actingUnit->x,
                                     MAP_STATE->actingUnit->y)) {
    AiUnit_JoinDamagedUnit();
  }
  if (MAP_STATE->actingUnit->serviceWanted != 1) {
    return;
  }
  if (MAP_STATE->actingUnit->type <= UNIT_MECH) {
    AiUnit_LaunchSilo();
  }
  gAiTurnState->flags |= 2;
  AiStep_DebugHook();
  AiUnit_GoToSupply();
}

/* Picks, among the units in reach that pass Tile_HasUnitToSupply and have
 * WANTS_SUPPLY set, the one with the least fuel, and supplies it from an
 * adjacent tile. Called by AiClass3_Act. */
void AiClass3_SupplyUnit(void) {
  MapUnit* unit;
  MapUnit* bestUnit = NULL;
  int bestX = -1;
  int bestY = -1;
  int bestFuel = 100;
  int x;
  int y;
  s16 pos[2];

  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      unit =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (Tile_HasUnitToSupply(x, y) && unit->serviceWanted == WANTS_SUPPLY &&
          bestFuel >= unit->fuel) {
        if (FindAttackTile(x, y, pos)) {
          bestFuel = unit->fuel;
          bestX = pos[0];
          bestY = pos[1];
          bestUnit = unit;
        }
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  bestUnit->serviceWanted = 0;
  AiUnit_PerformAction(bestX, bestY, AI_ACTION_SUPPLY, 0, 0);
}

/* Same search as AiClass3_SupplyUnit, with Tile_HasUnitToRepair as the filter:
 * picks the unit in reach with WANTS_SUPPLY set and the least fuel, and
 * performs the repair action from an adjacent tile. Called by AiBlackBoat_Act. */
void AiBlackBoat_RepairUnit(void) {
  MapUnit* unit;
  MapUnit* bestUnit = NULL;
  int bestX = -1;
  int bestY = -1;
  int bestFuel = 100;
  int x;
  int y;
  s16 pos[2];

  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      unit =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (Tile_HasUnitToRepair(x, y) && unit->serviceWanted == WANTS_SUPPLY &&
          bestFuel >= unit->fuel) {
        if (FindAttackTile(x, y, pos)) {
          bestFuel = unit->fuel;
          bestX = pos[0];
          bestY = pos[1];
          bestUnit = unit;
        }
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  bestUnit->serviceWanted = 0;
  AiUnit_PerformAction(
      bestX, bestY, AI_ACTION_REPAIR,
      Tile_GetDirection(bestX, bestY, bestUnit->x, bestUnit->y), 0);
}
