#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/ai/ai_tile_search.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/range_map.h"
#include "game/ai/ai_build_checks.h"
#include "game/ai/ai_count_units.h"
#include "game/ai/ai_unit_claims.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_transport_act.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_best_damage.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */
/* moves the acting unit toward (x, y) */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* RangeMap_FillMoves movement; guess: high enough to act as no limit. */
#define ANY_DISTANCE 110

/* MapTiles.rangeScratch here: lowest move cost of an own transport to the
 * tile; NO_TRANSPORT where none can get there. */
#define NO_TRANSPORT 127

#define UNLIMITED_MOVE 110

#define NO_TILE 0x7fff

/* MapTiles.rangeScratch2 bits read here, for each foot unit type; set by
 * AiTurn_BuildCaptureMap. */
#define TILE_FLAG_INFANTRY 0x80

#define TILE_FLAG_MECH 0x40

#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1)

typedef struct TilePos {
  s16 x;
  s16 y;
} TilePos;

extern u8 gUnitFitsLander[];

int UnitType_GetAttackKind(u8 unitType);

BOOL Player_IsInGame(u8 player);

BOOL Unit_IsVisibleToCurrentPlayer(u16 slot);

/* Damage percent the attacker type deals to a dived unit, 0 if it cannot hit it. */

/* Best damage percent of the attacker type against the target type. */

void RangeMap_FillMoves(int x, int y, int unitType, s16 maxCost, int moveRange);

BOOL UnitSlot_OwnsTile(s16 slot, s16 x, s16 y);

void RangeMap_FillUnitMoves(MapUnit* unit);

BOOL Unit_IsCurrentPlayers(s16 unitSlot);

BOOL Map_IsSiloOrCapturableTile(int x, int y);

void RangeMap_MarkAdjacentTiles(int arg);

/* Run by AiEmptyTransport_Act after AiEmptyTransport_GoToFootUnit, for an
 * empty transport: moves it towards the closest tile (by its own move cost)
 * within 2 Infantry moves of a base of the current player, so it is near
 * when new foot units are built. A base with a unit that already waited is
 * skipped. MapTiles.rangeScratch holds the Infantry distance to such a base,
 * -1 elsewhere. */
void AiEmptyTransport_WaitNearBase(void) {
  int bestMoves = NO_TILE;
  int bestX = -1;
  int bestY = 0;
  int baseX;
  int baseY;
  int nearX;
  int nearY;
  int x;
  int y;

  if (MAP_STATE->actingUnit->cargo[0] != 0) {
    return;
  }
  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_Fill(-1);
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  for (baseY = 0; baseY < MAP_STATE->unk_1c4->mapHeight; baseY++) {
    for (baseX = 0; baseX < MAP_STATE->unk_1c4->mapWidth; baseX++) {
      if (MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(baseX, baseY)]) !=
          TERRAIN_BASE) {
        continue;
      }
      if (MAP_STATE->unk_04 !=
          (MAP_TILES->tiles[MAP_TILE_INDEX(baseX, baseY)] & 0xe0)) {
        continue;
      }
      if (MAP_TILES->unitAt[MAP_TILE_INDEX(baseX, baseY)] != 0 &&
          (MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(baseX, baseY)]]
               .flags &
           MAP_UNIT_WAITED)) {
        continue;
      }
      RangeMap_FillMoves(baseX, baseY, UNIT_INFANTRY, 2, 1);
      for (nearY = 0; nearY < MAP_STATE->unk_1c4->mapHeight; nearY++) {
        for (nearX = 0; nearX < MAP_STATE->unk_1c4->mapWidth; nearX++) {
          if (MAP_STATE->rangeMap[nearY][nearX] >= 0) {
            MAP_TILES->rangeScratch[MAP_TILE_INDEX(nearX, nearY)] =
                MAP_STATE->rangeMap[nearY][nearX];
          }
        }
      }
    }
  }
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)] < 0) {
        continue;
      }
      if (MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (!Ai_CanEndMoveAt(x, y)) {
        continue;
      }
      if (Ai_IsTileNeededByAlly(x, y) == 1) {
        continue;
      }
      if (MAP_STATE->rangeMap[y][x] <= bestMoves) {
        bestX = x;
        bestY = y;
        bestMoves = MAP_STATE->rangeMap[y][x];
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}

/* A transport carrying a unit drops it for a capture. First pass: a free
 * property in reach (or next to it) that the cargo can capture, scored by
 * its rangeMap value plus a bonus per terrain (HQ highest), ties to the
 * lower threatMax + threatSum. Second pass, only for Infantry and Mech:
 * a free flagged tile with a property near it (within 3 tiles for
 * Infantry, 2 for Mech; with AI_STEP_ATTACK_DONE the tile must be a
 * property itself). Each pass that finds a tile sets the cargo's AI
 * target and queues the unload, so a second-pass result replaces the
 * first. */
void AiTransport_UnloadForCapture(void) {
  /* needed to match: declaration order sets registers and stack slots,
     * and the second pass needs its own loop variables */
  int unloadX;
  int x2;
  int bestScore;
  int unloadY;
  MapUnit* cargo;
  s16 stand[2];
  int standX;
  int threat;
  int standY;
  int captureX;
  int captureY;
  int bestThreat;
  int unloadX2;
  int y2;
  int unloadY2;
  int score;
  int standX2;
  int x;
  int y;
  int standY2;
  s16 capture[2];

  unloadX = -1;
  unloadY = -1;
  standX = -1;
  standY = -1;
  captureX = -1;
  captureY = -1;
  bestScore = -1;
  bestThreat = 0x7fff;
  cargo = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]];
  if (MAP_STATE->actingUnit->cargo[0] == 0) {
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (!Map_IsSiloOrCapturableTile(x, y)) {
        continue;
      }
      if (!AiUnit_FindStandTileNextTo(x, y, stand)) {
        continue;
      }
      if (MAP_STATE
                  ->unk_6088[MAP_STATE->mapTiles
                                 ->unk_1928[MAP_TILE_INDEX(x, y)]]
                  .claimCount != 0 &&
          !(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE) &&
          Ai_IsCaptureSiteTaken(x, y) == TRUE) {
        continue;
      }
      score = MAP_STATE->rangeMap[y][x];
      threat = MAP_STATE->mapTiles->threatMax[MAP_TILE_INDEX(x, y)] +
               MAP_STATE->mapTiles->threatSum[MAP_TILE_INDEX(x, y)];
      switch (
          MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)])) {
        case TERRAIN_HQ:
          score += 100;
          break;
        case TERRAIN_BASE:
          score += 80;
          break;
        case TERRAIN_SILO:
          score += 60;
          break;
        case TERRAIN_AIRPORT:
          score += 40;
          break;
        case TERRAIN_PORT:
        case TERRAIN_COM_TOWER:
          score += 20;
          break;
      }
      if (bestScore > score) {
        continue;
      }
      if (bestScore == score && bestThreat < threat) {
        continue;
      }
      bestScore = score;
      bestThreat = threat;
      standX = stand[0];
      standY = stand[1];
      unloadX = x;
      unloadY = y;
    }
  }
  if (bestScore != -1) {
    cargo->aiTargetX = unloadX;
    cargo->aiTargetY = unloadY;
    MAP_STATE
        ->unk_6088[MAP_STATE->mapTiles
                       ->unk_1928[MAP_TILE_INDEX(unloadX, unloadY)]]
        .claimCount++;
    AiUnit_PerformAction(standX, standY, AI_ACTION_UNLOAD,
                         Tile_GetDirection(standX, standY, unloadX, unloadY),
                         0);
  }

  unloadY2 = -1;
  unloadX2 = unloadY2;
  standY2 = unloadY2;
  standX2 = unloadY2;
  bestScore = unloadY2;
  bestThreat = 0x7fff;
  for (y2 = 0; y2 < MAP_STATE->unk_1c4->mapHeight; y2++) {
    for (x2 = 0; x2 < MAP_STATE->unk_1c4->mapWidth; x2++) {
      if (MAP_STATE->rangeMap[y2][x2] < 0) {
        continue;
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x2, y2)] != 0) {
        continue;
      }
      if (!AiUnit_FindStandTileNextTo(x2, y2, stand)) {
        continue;
      }
      switch (cargo->type) {
        case UNIT_INFANTRY: /* needed to match: the flag byte is read unsigned */
          if (!((u8)MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x2, y2)] &
                TILE_FLAG_INFANTRY)) {
            continue;
          }
          if (!(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) {
            if (!Ai_FindCaptureSiteNear(x2, y2, 3, capture)) {
              continue;
            }
          } else {
            if (!Map_IsSiloOrCapturableTile(x2, y2)) {
              continue;
            }
            capture[0] = x2;
            capture[1] = y2;
          }
          break;
        case UNIT_MECH:
          if (!((u8)MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x2, y2)] &
                TILE_FLAG_MECH)) {
            continue;
          }
          if (!(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) {
            if (!Ai_FindCaptureSiteNear(x2, y2, 2, capture)) {
              continue;
            }
          } else {
            if (!Map_IsSiloOrCapturableTile(x2, y2)) {
              continue;
            }
            capture[0] = x2;
            capture[1] = y2;
          }
          break;
        default:
          continue;
      }
      score = MAP_STATE->rangeMap[y2][x2];
      threat = MAP_STATE->mapTiles->threatMax[MAP_TILE_INDEX(x2, y2)] +
               MAP_STATE->mapTiles->threatSum[MAP_TILE_INDEX(x2, y2)];
      switch (MAP_TILE_TERRAIN(
          MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x2, y2)])) {
        case TERRAIN_HQ:
          score += 100;
          break;
        case TERRAIN_BASE:
          score += 80;
          break;
        case TERRAIN_SILO:
          score += 60;
          break;
        case TERRAIN_AIRPORT:
          score += 40;
          break;
        case TERRAIN_PORT:
        case TERRAIN_COM_TOWER:
          score += 20;
          break;
      }
      if (bestScore > score) {
        continue;
      }
      if (bestScore == score && bestThreat < threat) {
        continue;
      }
      bestScore = score;
      standX2 = stand[0];
      standY2 = stand[1];
      bestThreat = threat;
      unloadX2 = x2;
      captureX = capture[0];
      unloadY2 = y2;
      captureY = capture[1];
    }
  }
  if (bestScore == -1) {
    return;
  }
  cargo->aiTargetX = captureX;
  cargo->aiTargetY = captureY;
  MAP_STATE
      ->unk_6088[MAP_STATE->mapTiles
                     ->unk_1928[MAP_TILE_INDEX(captureX, captureY)]]
      .claimCount++;
  AiUnit_PerformAction(standX2, standY2, AI_ACTION_UNLOAD,
                       Tile_GetDirection(standX2, standY2, unloadX2, unloadY2),
                       0);
}

/* Run by AiLoadedTransport_Act twice: sends a transport that carries a unit
 * towards a capturable property or silo. The acting unit keeps its old
 * target when it can still reach it, no unit stands there and no other
 * unit claimed it (the claim does not count once AI_STEP_ATTACK_DONE is
 * set). Otherwise it picks the reachable site with the lowest score: its
 * rangeMap value plus 0 for an HQ, 2 for a base or silo, 3 for an airport,
 * 4 for a port or com tower and 6 for other properties; ties go to the
 * later tile in row order. The chosen site is claimed. */
void AiLoadedTransport_HeadForSite(void) {
  int bestScore;
  int x;
  int y;
  int bestX;
  int bestY;
  int score;
  int targetX;
  int targetY;

  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 0);
  targetX = MAP_STATE->actingUnit->aiTargetX;
  if (targetX != MAP_UNIT_NO_TARGET) {
    targetY = MAP_STATE->actingUnit->aiTargetY;
    if (MAP_STATE->rangeMap[targetY][targetX] >= 0 &&
        Map_IsSiloOrCapturableTile(targetX, targetY)) {
      if ((MAP_STATE
                   ->unk_6088[MAP_TILES
                                  ->unk_1928[MAP_TILE_INDEX(targetX, targetY)]]
                   .claimCount == 0 ||
           (gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) &&
          MAP_TILES->unitAt[MAP_TILE_INDEX(targetX, targetY)] == 0) {
        MAP_STATE
            ->unk_6088[MAP_TILES->unk_1928[MAP_TILE_INDEX(targetX, targetY)]]
            .claimCount++;
        AiUnit_MoveTowardTile(targetX, targetY);
      }
    }
  }
  bestX = -1;
  bestY = -1;
  bestScore = NO_TILE;
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
      score = MAP_STATE->rangeMap[y][x];
      switch (MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(x, y)])) {
        case TERRAIN_HQ:
          break;
        case TERRAIN_BASE:
          score += 2;
          break;
        case TERRAIN_SILO:
          score += 2;
          break;
        case TERRAIN_AIRPORT:
          score += 3;
          break;
        case TERRAIN_PORT:
        case TERRAIN_COM_TOWER:
          score += 4;
          break;
        default:
          score += 6;
          break;
      }
      if (bestScore >= score) {
        bestX = x;
        bestY = y;
        bestScore = score;
      }
    }
  }
  if (bestScore == NO_TILE) {
    return;
  }
  MAP_STATE->unk_6088[MAP_TILES->unk_1928[MAP_TILE_INDEX(bestX, bestY)]]
      .claimCount++;
  MAP_STATE->actingUnit->aiTargetX = bestX;
  MAP_STATE->actingUnit->aiTargetY = bestY;
  AiUnit_MoveTowardTile(bestX, bestY);
}

/* Boards the acting unit on an own Black Boat or Lander it can reach that has
 * a free second cargo slot and can carry its type. A Black Boat beats a
 * Lander; later tiles win a tie. Marks the transport's AI target as its own
 * tile and moves the unit onto it. */
void AiUnit_BoardSeaTransport(void) {
  u8 type;
  int bestX;
  MapUnit* unit;
  int bestY;
  u8 bestType;
  int x;
  int y;
  bestX = -1;
  bestY = -1;
  bestType = 0;

  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] == 0 ||
          !Unit_IsCurrentPlayers(
              MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(x, y)])) {
        continue;
      }
      unit =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      type = unit->type;
      if (type != UNIT_BLACK_BOAT && type != UNIT_LANDER) {
        continue;
      }
      if ((Ov000_gUnitStats[type].carryTypes +
           MAP_STATE->actingUnit->type)[1] == 0) {
        continue;
      }
      if (unit->cargo[1] != 0) {
        continue;
      }
      switch (bestType) {
        case UNIT_NONE:
        case UNIT_LANDER:
          break;
        case UNIT_BLACK_BOAT:
          if (type == UNIT_LANDER) {
            continue;
          }
          break;
        default:
          continue;
      }
      bestX = x;
      bestY = y;
      bestType = unit->type; /* needed to match: not type */
    }
  }
  if (bestX == -1) {
    return;
  }
  MAP_STATE->actingUnit->transportWanted = 0;
  if (MAP_STATE->actingUnit->aiOrder == 2 ||
      MAP_STATE->actingUnit->aiOrder == 6) {
    MAP_STATE->actingUnit->aiOrder = 0;
    MAP_STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
    MAP_STATE->actingUnit->aiTargetY = MAP_UNIT_NO_TARGET;
  }
  unit =
      &MAP_STATE
           ->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(bestX, bestY)]];
  unit->aiTargetX = bestX;
  unit->aiTargetY = bestY;
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(bestX, bestY, AI_ACTION_BOARD, 0, 0);
}

/* Sends a ground unit with over 60 hp to meet an own Lander, or a Black
 * Boat for Infantry and Mech, on a shoal or port. Each transport with
 * room is checked; one that already carries a unit counts only for an
 * armed unit that can reach it. An armed unit only meets on tiles it can
 * reach; an unarmed one may meet anywhere. The meeting tile with the
 * lowest sum of the unit's and the transport's move costs is chosen.
 * With no transport that can get to a tile the unit reaches, the unit
 * heads for the nearest port its player owns. */
void AiUnit_SeekSeaTransport(void) {
  /* needed to match: this declaration order and one pair of loop
     * variables per loop give the original register use */
  int reach;
  int portX;
  int x;
  int meetX;
  int portY;
  int meetY;
  int bestX;
  MapUnit* unit;
  int terrain;
  int bestY;
  int type;
  int y;
  int best;
  BOOL found;
  BOOL unarmed;
  int i;

  best = 0x7fff;
  bestX = -1;
  bestY = -1;
  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return;
  }
  if (!AiProfile_WantsSeaTransport() && Ai_CountUnitsOfType(UNIT_LANDER) == 0 &&
      Ai_CountUnitsOfType(UNIT_BLACK_BOAT) == 0) {
    return;
  }
  if (MAP_STATE->actingUnit->claimedLandmass) {
    return;
  }
  if (MAP_STATE->actingUnit->hp < 61) {
    return;
  }
  if (gUnitFitsLander[MAP_STATE->actingUnit->type] == 0) {
    return;
  }
  if (AiUnit_ClaimLandmass() == TRUE) {
    return;
  }
  AiUnit_BoardSeaTransport();
  unarmed =
      UnitType_GetAttackKind(MAP_STATE->actingUnit->type) == ATTACK_KIND_NONE;
  MAP_STATE->actingUnit->transportWanted = 2;
  if (!unarmed) {
    RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch2);
    RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                       MAP_STATE->actingUnit->type, ANY_DISTANCE, 0);
    RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_Fill(NO_TRANSPORT);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);

  found = FALSE;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    type = unit->type;
    if (type == UNIT_NONE) {
      continue;
    }
    if (MAP_STATE->actingUnit->type <= UNIT_MECH) {
      if (type != UNIT_LANDER && type != UNIT_BLACK_BOAT) {
        continue;
      }
    } else if (type != UNIT_LANDER) {
      continue;
    }
    if (unit->cargo[1] != 0) {
      continue;
    }
    if (unit->cargo[0] != 0) {
      if (unarmed == TRUE) {
        continue;
      }
      if (MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(unit->x, unit->y)] <
          0) {
        continue;
      }
    }
    RangeMap_FillMoves(unit->x, unit->y, type, ANY_DISTANCE, 0);
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        reach = MAP_STATE->rangeMap[y][x];
        if (reach < 0) {
          continue;
        }
        terrain =
            MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]);
        if (terrain != TERRAIN_SHOAL && terrain != TERRAIN_PORT) {
          continue;
        }
        if (reach >= MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(x, y)]) {
          continue;
        }
        if (!unarmed &&
            MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] < 0) {
          continue;
        }
        MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(x, y)] = reach;
        found = TRUE;
      }
    }
  }
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, ANY_DISTANCE, 0);

  if (!found) {
    for (portY = 0; portY < MAP_STATE->unk_1c4->mapHeight; portY++) {
      for (portX = 0; portX < MAP_STATE->unk_1c4->mapWidth; portX++) {
        if (MAP_STATE->rangeMap[portY][portX] < 0) {
          continue;
        }
        if (MAP_TILE_TERRAIN(
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(portX, portY)]) !=
            TERRAIN_PORT) {
          continue;
        }
        if (!UnitSlot_OwnsTile(MAP_STATE->currentUnitBase, portX, portY)) {
          continue;
        }
        reach = MAP_STATE->rangeMap[portY][portX];
        if (best >= reach) {
          bestX = portX;
          bestY = portY;
          best = reach;
        }
      }
    }
    if (bestX == -1) {
      return;
    }
    AiUnit_MoveTowardTile(bestX, bestY);
    return;
  }

  for (meetY = 0; meetY < MAP_STATE->unk_1c4->mapHeight; meetY++) {
    for (meetX = 0; meetX < MAP_STATE->unk_1c4->mapWidth; meetX++) {
      if (MAP_STATE->rangeMap[meetY][meetX] < 0) {
        continue;
      }
      terrain = MAP_TILE_TERRAIN(
          MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(meetX, meetY)]);
      if (terrain != TERRAIN_SHOAL && terrain != TERRAIN_PORT) {
        continue;
      }
      if (MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(meetX, meetY)] ==
          NO_TRANSPORT) {
        continue;
      }
      reach = MAP_STATE->rangeMap[meetY][meetX] +
              MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(meetX, meetY)];
      if (best >= reach) {
        bestX = meetX;
        bestY = meetY;
        best = reach;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}

/* Checks the tile found and may move it; FALSE means the tile is not usable. */
/* Looks for an enemy unit the acting unit can reach this turn and hurt for
 * at least 20 percent. Prefers the nearest one it hurts for 50 or more, else
 * the nearest of the weaker ones. A Cruiser counts a Sub as a quarter of the
 * distance. On success writes the unit's tile to tile and returns TRUE. */
BOOL AiUnit_FindEnemyTarget(s16* tile) {
  int player;
  int bestWeakCost;
  int score;
  int bestStrongCost;
  TilePos strong;
  int cost;
  int i;
  MapUnit* unit;
  TilePos weak;
  int y;
  int x;

  bestStrongCost = 0x7fff;
  bestWeakCost = 0x7fff;

  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player) ||
        MAP_STATE->players[MAP_STATE->currentPlayer].team ==
            MAP_STATE->players[player].team) {
      continue;
    }
    for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
         i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
      unit = &MAP_STATE->units[i];
      if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED) ||
          MAP_STATE->rangeMap[unit->y][unit->x] < 0) {
        continue;
      }
      if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) != 0 &&
          !Player_CanSeeUnitAt(MAP_STATE->currentPlayer, unit->x, unit->y)) {
        continue;
      }
      /* Subs and Stealths are hidden unless the player can see them; a Cruiser always finds a Sub. */
      if ((unit->type == UNIT_SUB &&
           MAP_STATE->actingUnit->type != UNIT_CRUISER) ||
          unit->type == UNIT_STEALTH) {
        if (!Unit_IsVisibleToCurrentPlayer(i)) {
          continue;
        }
      }
      if ((unit->flags & MAP_UNIT_DIVED) &&
          !Ai_BestDamageVsDived(MAP_STATE->actingUnit->type, unit->type, 0)) {
        continue;
      }
      score = Ai_BestDamage(MAP_STATE->actingUnit->type, unit->type, 0);
      y = unit->y;
      x = unit->x;
      cost = MAP_STATE->rangeMap[y][x];
      if (MAP_STATE->actingUnit->type == UNIT_CRUISER &&
          unit->type == UNIT_SUB) {
        cost >>= 2;
      }
      if (score < 20) {
        continue;
      }
      if (score < 50) {
        if (cost <= bestWeakCost) {
          bestWeakCost = cost;
          weak.x = unit->x;
          weak.y = unit->y;
        }
      } else if (cost <= bestStrongCost) {
        strong.x = unit->x;
        bestStrongCost = cost;
        strong.y = unit->y;
      }
    }
  }
  if (bestStrongCost == 0x7fff) {
    if (bestWeakCost == 0x7fff) {
      return 0;
    }
    strong = weak;
  }
  if (!AiUnit_FindFiringTile(&strong.x)) {
    return 0;
  }
  tile[0] = strong.x;
  tile[1] = strong.y;
  return 1;
}

/* Looks for a target tile (AiUnit_FindEnemyTarget) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void AiUnit_MoveToEnemyTarget(void) {
  s16 tile[2];

  if (AiUnit_FindEnemyTarget(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}
