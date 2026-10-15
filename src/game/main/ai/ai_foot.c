#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/strike_target.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_tile_search.h"
#include "game/math.h"
#include "game/map_unit.h"
#include "game/ai/ai_move_search.h"
#include "game/ai/ai_foot.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_unit_claims.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* capture points the unit adds per turn */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define ATTACK_KIND_INDIRECT 1
/* the site entry for the tile the acting unit is heading to */
#define TARGET_SITE                                        \
  (MAP_STATE->unk_6088[MAP_TILES->unk_1928[MAP_TILE_INDEX( \
      MAP_STATE->actingUnit->aiTargetX, MAP_STATE->actingUnit->aiTargetY)]])
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1)
#define NO_TILE 0x7fff
/* MapUnit.transportWanted: the transport a foot unit waits for.
 * AiEmptyTransport_GoToFootUnit sends APCs to 1, T-Copters to 1 to 3. */
#define WANTS_APC 1
#define WANTS_T_COPTER 2

int UnitType_GetAttackKind(u8 unitType);
/* 0 direct, 1 indirect, 2 unarmed */
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiAttack_Score(int arg, s16* pos);
int Unit_CaptureEndsNextTurn(MapUnit* unit);
int Unit_CapturePoints(MapUnit* unit);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
BOOL Terrain_IsHqBaseAirportPortOrTower(u8 tile);
BOOL Map_IsSiloOrCapturableTile(int x, int y);
/* called when no site is in range */
int Unit_CaptureTurnsLeft(MapUnit* unit);
BOOL Map_IsTileOwnedByTeamOf(int player, int x, int y);

/* Infantry and mechs only: attacks the first visible enemy infantry or mech
 * whose capture of an HQ, base or airport (neutral or held by our team) would
 * finish next turn. */
void AiFoot_AttackCapturingUnit(void) {
  int x;
  int y;
  u8 tile;
  int terrain;
  MapUnit* unit;
  s16 pos[2];

  if (MAP_STATE->actingUnit->type > UNIT_MECH) {
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)];
      terrain = MAP_TILE_TERRAIN(tile);
      if (terrain != TERRAIN_HQ && terrain != TERRAIN_BASE &&
          terrain != TERRAIN_AIRPORT) {
        continue;
      }
      /* owner bits; needed to match: MAP_TILE_OWNER adds a mask */
      if ((tile >> 5) != 0 &&
          !Map_IsTileOwnedByTeamOf(MAP_STATE->currentPlayer, x, y)) {
        continue;
      }
      unit =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      if (unit->type == UNIT_NONE || unit->type > UNIT_MECH) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, unit)].team) {
        continue;
      }
      if (!Unit_CaptureEndsNextTurn(unit)) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      Ai_ClearActingUnitTarget();
      AiUnit_PerformAction(pos[0], pos[1], AI_ACTION_ATTACK_UNIT,
                           MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)],
                           0);
      return;
    }
  }
}

/* Last step of AiFoot_ContinueCapture: an Infantry or Mech that has started
 * capturing keeps capturing where it stands. */
void AiFoot_KeepCapturing(void) {
  if (MAP_STATE->actingUnit->type > UNIT_MECH ||
      MAP_STATE->actingUnit->captureProgress == 0) {
    return;
  }
  if (Unit_CaptureTurnsLeft(MAP_STATE->actingUnit) >= 2) {
    MAP_STATE
        ->unk_6088[MAP_STATE->mapTiles->unk_1928[MAP_TILE_INDEX(
            MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y)]]
        .claimCount++;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                       AI_ACTION_CAPTURE, 0, 0);
}

/* turns the unit needs to capture a property; 20 at 0 hp */
/* Infantry and mechs: picks a free property tile in reach to capture, scored by
 * its rangeMap value plus a bonus per terrain, or a missile silo to launch. A unit
 * that would need 5 or more turns to capture only looks at silos. Ties go to
 * the tile with the lower threatMax + threatSum, then to
 * the roll. */
void AiFoot_PickCaptureTarget(void) {
  /* needed to match: declaration order sets the registers */
  int captureX;
  int captureY;
  int siloX;
  int siloY;
  int bestScore;
  int bestThreat;
  int siloScore;
  int terrain;
  int x;
  int y;
  int siloOnly;
  int score;
  int threat;
  s16 site[2];

  if (MAP_STATE->actingUnit->type > UNIT_MECH) {
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  captureY = 9999;
  captureX = captureY;
  siloY = captureY;
  siloX = captureY;
  bestScore = -1;
  siloScore = bestScore;
  bestThreat = 0x7fff;
  siloOnly = Unit_CaptureTurnsLeft(MAP_STATE->actingUnit) >= 5;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      terrain =
          MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]);
      if (siloOnly == 1 && terrain != TERRAIN_SILO) {
        continue;
      }
      if (!Map_IsSiloOrCapturableTile(x, y)) {
        continue;
      }
      if (x == MAP_STATE->actingUnit->aiTargetX &&
          y == MAP_STATE->actingUnit->aiTargetY) {
        score = 4; /* the tile the unit was already heading for */
      } else {
        score = MAP_STATE->rangeMap[y][x];
      }
      switch (terrain) {
        case TERRAIN_HQ:
          score += 25;
          break;
        case TERRAIN_BASE:
          score += 20;
          break;
        case TERRAIN_SILO:
          score += 15;
          break;
        case TERRAIN_AIRPORT:
          score += 10;
          break;
        case TERRAIN_PORT:
        case TERRAIN_COM_TOWER:
          score += 5;
          break;
      }
      if (terrain != TERRAIN_SILO) {
        if (bestScore > score) {
          continue;
        }
        threat = MAP_STATE->mapTiles->threatMax[MAP_TILE_INDEX(x, y)] +
                 MAP_STATE->mapTiles->threatSum[MAP_TILE_INDEX(x, y)];
        if (bestScore == score) {
          if (bestThreat < threat) {
            continue;
          }
          if (bestThreat == threat && gAiTurnState->roll < 50) {
            continue;
          }
        }
        captureX = x;
        bestScore = score;
        bestThreat = threat;
        captureY = y;
      } else if (siloScore <= score) {
        siloX = x;
        siloScore = score;
        siloY = y;
      }
    }
  }
  if (siloScore > bestScore) {
    site[0] = siloX;
    site[1] = siloY;
    if (Strike_FindBestSite(site, MAP_STATE->currentPlayer, 1,
                            STRIKE_MODE_AI_FUNDS) == 1) {
      AiUnit_PerformAction(siloX, siloY, AI_ACTION_LAUNCH_SILO, site[0],
                           site[1]);
      return;
    }
  }
  if (bestScore == -1) {
    return;
  }
  if (Unit_CaptureTurnsLeft(MAP_STATE->actingUnit) >= 2) {
    MAP_STATE
        ->unk_6088[MAP_STATE->mapTiles
                       ->unk_1928[MAP_TILE_INDEX(captureX, captureY)]]
        .claimCount++;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(captureX, captureY, AI_ACTION_CAPTURE, 0, 0);
}

/* Run twice by AiFoot_MoveAndAttack for the acting Infantry or Mech:
 * heads for a capturable property or silo in its range map.
 *
 * A unit above 60 hp first keeps its old target if it can still reach it,
 * no other unit claimed it and no unit stands there. Then it picks the
 * tile with the lowest score (2 * MapTiles.rangeScratch, minus 1 for an HQ,
 * base, airport, port or com tower), ties going to the higher rangeMap
 * value. On the first run tiles that another foot unit claimed
 * (Unk_02080440_6088.claimCount) are skipped; once AI_STEP_ATTACK_DONE is
 * set they are not. A unit above 60 hp claims its tile, and asks for an
 * APC when the tile is more than 2 away; with no tile left it asks for a
 * T-Copter. */
void AiFoot_HeadForCaptureSite(void) {
  int i;
  MapUnit* unit;
  MapUnit* acting;
  int targetX;
  int targetY;
  int bestX;
  int bestY;
  int bestRange;
  int bestScore;
  int range;
  int score;
  int x;
  int y;

  /* Clear unk_0c_4 when an own Lander or Black Boat is in move range. */
  if ((gAiTurnState->domainsOwn & AI_DOMAIN_SEA) &&
      MAP_STATE->actingUnit->unk_0c_4 == 1) {
    RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
    for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
      unit = &MAP_STATE->units[i];
      /* needed to match: the UNIT_NONE test comes first */
      if (unit->type != UNIT_NONE &&
          (unit->type == UNIT_LANDER || unit->type == UNIT_BLACK_BOAT)) {
        if (MAP_STATE->rangeMap[unit->y][unit->x] >= 0) {
          MAP_STATE->actingUnit->unk_0c_4 = 0;
          break;
        }
      }
    }
  }
  acting = MAP_STATE->actingUnit;
  RangeMap_FillMovesOverTurns(acting->x, acting->y, acting->type,
                              AiActingUnit_GetSearchMovement());
  acting = STATE_IDX_FIRST->actingUnit;
  targetX = acting->aiTargetX;
  if (targetX != MAP_UNIT_NO_TARGET && acting->hp >= 61) {
    targetY = acting->aiTargetY;
    if (MAP_STATE->rangeMap[targetY][targetX] >= 0 &&
        Map_IsSiloOrCapturableTile(targetX, targetY)) {
      if (MAP_STATE
                  ->unk_6088[MAP_TILES
                                 ->unk_1928[MAP_TILE_INDEX(targetX, targetY)]]
                  .claimCount == 0 &&
          MAP_TILES->unitAt[MAP_TILE_INDEX(targetX, targetY)] == 0) {
        MAP_STATE
            ->unk_6088[MAP_TILES->unk_1928[MAP_TILE_INDEX(targetX, targetY)]]
            .claimCount++;
        if (MAP_TILES->rangeScratch[MAP_TILE_INDEX(targetX, targetY)] > 2) {
          AiFoot_BoardTransportForSite(
              MAP_TILES->rangeScratch[MAP_TILE_INDEX(targetX, targetY)]);
        }
        AiUnit_MoveTowardTile(targetX, targetY);
      }
    }
  }
  bestX = -1;
  bestY = -1;
  bestRange = NO_TILE;
  bestScore = bestRange;
  MAP_STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
  MAP_STATE->actingUnit->aiTargetY = MAP_UNIT_NO_TARGET;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Map_IsSiloOrCapturableTile(x, y)) {
        continue;
      }
      if (MAP_STATE->unk_6088[MAP_TILES->unk_1928[MAP_TILE_INDEX(x, y)]]
                  .claimCount != 0 &&
          !(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) {
        continue;
      }
      range = MAP_STATE->rangeMap[y][x];
      score = MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)] * 2;
      if (Terrain_IsHqBaseAirportPortOrTower(MAP_TILE_TERRAIN(
              MAP_TILES->tiles[MAP_TILE_INDEX(x, y)])) == TRUE) {
        score--;
      }
      if (score > bestScore) {
        continue;
      }
      if (score == bestScore && bestRange > range) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestRange = range;
      bestScore = score;
    }
  }
  if (bestRange != NO_TILE) {
    if (MAP_STATE->actingUnit->hp >= 61) {
      MAP_STATE->unk_6088[MAP_TILES->unk_1928[MAP_TILE_INDEX(bestX, bestY)]]
          .claimCount++;
    }
    MAP_STATE->actingUnit->aiTargetX = bestX;
    MAP_STATE->actingUnit->aiTargetY = bestY;
    if ((bestScore >> 1) > 2 && MAP_STATE->actingUnit->hp >= 61 &&
        !MAP_STATE->actingUnit->claimedLandmass) {
      AiFoot_BoardTransportForSite(bestScore >> 1);
      MAP_STATE->actingUnit->transportWanted = WANTS_APC;
    }
    AiUnit_MoveTowardTile(bestX, bestY);
  } else if (MAP_STATE->actingUnit->hp >= 61 &&
             !MAP_STATE->actingUnit->claimedLandmass) {
    AiFoot_BoardTransport();
    MAP_STATE->actingUnit->transportWanted = WANTS_T_COPTER;
  }
}

/* Called by AiFoot_HeadForCaptureSite when the capture site is more than 2
 * tiles away (siteDistance). Like AiFoot_BoardTransport, but the unit also
 * gives up its claim on its old target site (Unk_02080440_6088.claimCount) when
 * it takes a boat. A Lander or Black Boat is skipped when the unit has
 * unk_0c_4 set or the site is 4 tiles or less away, and a Black Boat or
 * Lander holding the pick is not replaced by an APC. */
void AiFoot_BoardTransportForSite(int siteDistance) {
  int boatX = -1;
  int boatY = -1;
  int boatType = 0;
  int x;
  int y;
  MapUnit* transport;
  int type;

  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] == 0) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(MAP_TILES->unk_508[MAP_TILE_INDEX(x, y)])) {
        continue;
      }
      transport = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)]];
      type = transport->type;
      switch (type) {
        case UNIT_APC:
        case UNIT_T_COPTER:
          if (transport->cargo[0] != 0) {
            continue;
          }
          break;
        case UNIT_BLACK_BOAT:
        case UNIT_LANDER:
          if (transport->cargo[1] != 0) {
            continue;
          }
          break;
        default:
          continue;
      }
      switch (boatType) {
        case UNIT_NONE:
        case UNIT_APC:
          break;
        case UNIT_BLACK_BOAT:
          if (type == UNIT_LANDER) {
            continue;
          }
        case UNIT_LANDER:
          if (MAP_STATE->actingUnit->unk_0c_4 == 1) {
            continue;
          }
          if (type == UNIT_APC) {
            continue;
          }
          break;
        case UNIT_T_COPTER:
        default:
          continue;
      }
      if (MAP_STATE->actingUnit->unk_0c_4 == 1 || siteDistance <= 4) {
        if (type == UNIT_LANDER || type == UNIT_BLACK_BOAT) {
          continue;
        }
      }
      boatX = x;
      boatY = y;
      boatType = transport->type; /* needed to match: not type */
    }
  }
  if (boatX == -1) {
    return;
  }
  if (boatType == UNIT_APC) {
    if (AiUnit_CanReachCaptureSite(
            &MAP_STATE
                 ->units[MAP_TILES->unitAt[MAP_TILE_INDEX(boatX, boatY)]]) ==
        0) {
      return;
    }
  }
  if (boatType == UNIT_LANDER || boatType == UNIT_BLACK_BOAT) {
    if (MAP_STATE->actingUnit->aiTargetX != MAP_UNIT_NO_TARGET) {
      if (TARGET_SITE.claimCount != 0) {
        TARGET_SITE.claimCount--;
      }
    }
    transport =
        &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(boatX, boatY)]];
    transport->aiTargetX = boatX;
    transport->aiTargetY = boatY;
  }
  MAP_STATE->actingUnit->transportWanted = 0;
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(boatX, boatY, AI_ACTION_BOARD, 0, 0);
}

/* Called by AiFoot_HeadForCaptureSite when no capture site is in range.
 * The acting foot unit boards an own transport with room that it can
 * reach: an APC, T-Copter, Black Boat or Lander. Later finds replace
 * earlier ones, except that a T-Copter is never replaced, a Lander is not
 * replaced by an APC, and a Black Boat is replaced only by another Black
 * Boat or a T-Copter. An APC is taken only if AiUnit_CanReachCaptureSite accepts it. A
 * boat gets the tile as its AI target. */
void AiFoot_BoardTransport(void) {
  int boatX = -1;
  int boatY = -1;
  int boatType = 0;
  int x;
  int y;
  MapUnit* transport;
  int type;

  if (MAP_STATE->actingUnit->claimedLandmass || AiUnit_ClaimLandmass() == 1) {
    MAP_STATE->actingUnit->transportWanted = 0;
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] == 0) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(MAP_TILES->unk_508[MAP_TILE_INDEX(x, y)])) {
        continue;
      }
      transport = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)]];
      type = transport->type;
      switch (type) {
        case UNIT_APC:
        case UNIT_T_COPTER:
          if (transport->cargo[0] != 0) {
            continue;
          }
          break;
        case UNIT_BLACK_BOAT:
        case UNIT_LANDER:
          if (transport->cargo[1] != 0) {
            continue;
          }
          break;
        default:
          continue;
      }
      switch (boatType) {
        case UNIT_NONE:
        case UNIT_APC:
          break;
        case UNIT_BLACK_BOAT:
          if (type == UNIT_LANDER) {
            continue;
          }
        case UNIT_LANDER:
          if (type == UNIT_APC) {
            continue;
          }
          break;
        case UNIT_T_COPTER:
        default:
          continue;
      }
      boatX = x;
      boatY = y;
      boatType = type;
    }
  }
  if (boatX == -1) {
    return;
  }
  if (boatType == UNIT_APC) {
    if (AiUnit_CanReachCaptureSite(
            &MAP_STATE
                 ->units[MAP_TILES->unitAt[MAP_TILE_INDEX(boatX, boatY)]]) ==
        0) {
      return;
    }
  }
  if (boatType == UNIT_LANDER || boatType == UNIT_BLACK_BOAT) {
    transport =
        &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(boatX, boatY)]];
    transport->aiTargetX = boatX;
    transport->aiTargetY = boatY;
  }
  MAP_STATE->actingUnit->transportWanted = 0;
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(boatX, boatY, AI_ACTION_BOARD, 0, 0);
}

/* Joins the acting unit with one of our own units of the same type that is
 * capturing and not at full hp, on any tile the acting unit can reach. HP here
 * is the shown value, 1-10. The join is queued when the sum is at most 15 on
 * an HQ, airport, port, base or com tower, at most 12 when the acting unit
 * shows under 7, or at most 10. */
void AiFoot_JoinCapturingUnit(void) {
  MapUnit* unit;
  int x;
  int y;
  int slot;
  int targetHp;
  int ownHp;

  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      if (slot == 0) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      if (unit->captureProgress == 0) {
        continue;
      }
      if (unit->type != MAP_STATE->actingUnit->type) {
        continue;
      }
      if (unit->hp == 0) {
        targetHp = 0;
      } else {
        targetHp = HwDiv32(unit->hp - 1, 10) + 1;
      }
      if (targetHp == 10) {
        continue;
      }
      if (MAP_STATE->actingUnit->hp == 0) {
        ownHp = 0;
      } else {
        ownHp = HwDiv32(MAP_STATE->actingUnit->hp - 1, 10) + 1;
      }
      if (Terrain_IsHqBaseAirportPortOrTower(
              MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(unit->x, unit->y)]) &&
          targetHp + ownHp <= 15) {
        AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
      }
      if (ownHp < 7 && targetHp + ownHp <= 12) {
        AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
      }
      if (targetHp + ownHp <= 10) {
        AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
      }
    }
  }
}

/* Looser form of AiFoot_JoinCapturingUnit, run by AiFoot_MoveAndAttack: joins
 * a same-type own unit that is capturing and shows under 10 hp, when the two
 * shown hp values add up to 15 or less and at least one of them is under 7.
 * There is no terrain test. */
void AiFoot_JoinCapturingUnitLoose(void) {
  MapUnit* unit;
  int slot;
  int x;
  int y;
  int targetHp;
  int ownHp;

  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      if (slot == 0) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      if (unit->captureProgress == 0) {
        continue;
      }
      if (unit->type != MAP_STATE->actingUnit->type) {
        continue;
      }
      if (unit->hp == 0) {
        targetHp = 0;
      } else {
        targetHp = HwDiv32(unit->hp - 1, 10) + 1;
      }
      if (targetHp == 10) {
        continue;
      }
      if (MAP_STATE->actingUnit->hp == 0) {
        ownHp = 0;
      } else {
        ownHp = HwDiv32(MAP_STATE->actingUnit->hp - 1, 10) + 1;
      }
      if (targetHp >= 7 && ownHp >= 7) {
        continue;
      }
      if (targetHp + ownHp <= 15) {
        AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
      }
    }
  }
}

/* Infantry and mechs. Among the tiles in reach, notes the free missile silo
 * with the lowest rangeMap value and picks the enemy unit with the best
 * AiAttack_Score. Vehicles are targets only when indirect, or for a mech with
 * ammo. Infantry and mechs are targets when their capture ends next turn, or
 * when they have over 30 hp, are capturing or have over 60 hp, and the acting
 * unit adds under 12 capture points a turn. When AiUnit_TeammateOutdamages holds for the
 * target, it also needs two or more open sides the acting unit can reach.
 * A silo with a strike site worth launching at is chosen over the attack. */
void AiFoot_PickAttackTarget(void) {
  int maxY;
  int minX;
  int attackX;
  int attackY;
  int siloX;
  int siloY;
  int siloRank;
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
  int tile;

  attackX = -1;
  bestScore = 0;
  attackY = -1;
  siloX = -1;
  siloY = -1;
  bestSlot = 0;
  siloRank = 110;
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = MAP_TILE_INDEX(x, y);
      if (MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[tile]) == TERRAIN_SILO &&
          MAP_STATE->mapTiles->unitAt[tile] == 0) {
        if (MAP_STATE->rangeMap[y][x] < siloRank) {
          siloRank = MAP_STATE->rangeMap[y][x];
          siloX = x;
          siloY = y;
        }
      }
      target = &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[tile]];
      if (target->type == UNIT_NONE) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, target)].team) {
        continue;
      }
      if (target->type > UNIT_MECH) {
        if (UnitType_GetAttackKind(target->type) != ATTACK_KIND_INDIRECT) {
          if (MAP_STATE->actingUnit->type == UNIT_INFANTRY) {
            continue;
          }
          if (MAP_STATE->actingUnit->ammo == 0) {
            continue;
          }
        }
      } else if (!Unit_CaptureEndsNextTurn(target)) {
        if (target->hp <= 30) {
          continue;
        }
        if (target->captureProgress == 0 && target->hp <= 60) {
          continue;
        }
        if (Unit_CapturePoints(MAP_STATE->actingUnit) >= 12) {
          continue;
        }
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      score = AiAttack_Score(MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)],
                             pos);
      if (score == 0) {
        continue;
      }
      /* infantry against a vehicle: the target has to lose 15 hp or die */
      if (MAP_STATE->actingUnit->type == UNIT_INFANTRY &&
          target->type > UNIT_MECH && MAP_STATE->combatants[1].hp != 0 &&
          MAP_STATE->combatants[1].hpLost < 15) {
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
            /* the target's four sides */
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
      bestScore = score;
      attackX = pos[0];
      attackY = pos[1];
      bestSlot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
    }
  }
  if (siloX != -1) {
    pos[0] = siloX;
    pos[1] = siloY;
    if (Strike_FindBestSite(pos, MAP_STATE->currentPlayer, 1,
                            STRIKE_MODE_AI_FUNDS) == 1) {
      AiUnit_PerformAction(siloX, siloY, AI_ACTION_LAUNCH_SILO, pos[0], pos[1]);
      return;
    }
  }
  if (bestScore == 0) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(attackX, attackY, AI_ACTION_ATTACK_UNIT, bestSlot, 0);
}
