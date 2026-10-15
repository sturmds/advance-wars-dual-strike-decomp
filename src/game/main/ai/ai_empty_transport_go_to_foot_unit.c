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
#include "game/ai.h"

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

extern u8 gTerrainFactoryKind[];

/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
extern u8 gAiTransportsSentToUnit[];

/* Needed to match: this file passes the movement as an int, with no s16
 * narrowing, so it declares RangeMap_FillMoves with an int. */
void RangeMap_FillMoves(int x, int y, int unitType, int movement,
                        int moveRange);

BOOL Map_IsSiloOrCapturableTile(int x, int y);

int Player_GetUnitMoveRange(int player, int unitType);

int Unit_GetUsableMovement(MapUnit* unit);

/* move points this turn, capped by fuel */

int Unit_IsTileOwnedByTeam(int unitIndex, int tile);

/* Run by AiEmptyTransport_Act for an empty APC or T-Copter (the acting
 * unit). Picks one of the current player's Infantry or Mechs that no other
 * transport is heading for, and the tile where the two can meet: one the
 * foot unit can reach, empty, where the transport can stop, and not a
 * silo, a capturable property or the team's own base, airport or port.
 * Of those it takes the tile closest to the transport. If the transport
 * gets there this turn it moves there and waits, otherwise
 * AiUnit_MoveTowardTile moves it towards the tile.
 *
 * MapTiles.rangeScratch: transport's move cost to each tile.
 * MapTiles.rangeMapTiles: foot unit's move cost (where rangeMap points).
 * MapTiles.rangeScratch2: second fill from the transport, used to break ties
 * when the foot unit has an AI target. */
void AiEmptyTransport_GoToFootUnit(void) {
  int meetX = -1;
  int meetY = -1;
  int unitTileX = -1;
  int unitTileY = -1;
  int meetDist = NO_TILE;
  int meetSlot = 0;
  int unitTileDist;
  int transportReach;
  int nearTarget;
  int dist;
  int meetRange;
  MapUnit* unit;
  int i;
  int unitTileAlt;
  int x;
  int y;
  int tileDist;
  int tileAlt;
  int footMove;

  if (MAP_STATE->actingUnit->cargo[0] != 0) {
    return;
  }
  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  transportReach = Unit_GetUsableMovement(MAP_STATE->actingUnit) + 1;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type == UNIT_NONE || unit->type > UNIT_MECH) {
      continue;
    }
    if (gAiTransportsSentToUnit[i & 63] != 0) {
      continue;
    }
    if (unit->claimedLandmass) {
      continue;
    }
    /* APCs only go for foot units marked 1, T-Copters for 1 to 3 */
    switch (MAP_STATE->actingUnit->type) {
      case UNIT_APC:
        if (unit->transportWanted != 1) {
          continue;
        }
        break;
      case UNIT_T_COPTER:
        if (unit->transportWanted == 0) {
          continue;
        }
        break;
    }
    footMove = Player_GetUnitMoveRange(MAP_STATE->currentPlayer, unit->type);
    dist = MAP_TILES->rangeScratch[MAP_TILE_INDEX(unit->x, unit->y)];
    nearTarget = 0;
    meetRange = transportReach + footMove;
    /* Meaning of these two checks unknown; when one holds, the unit is
         * treated as just out of reach. */
    if (dist < meetRange) {
      if (func_020a507c(unit->x, unit->y, footMove * 2 - 1) == 1 ||
          func_020a50d4(unit->x, unit->y, unit->type, footMove) == 1) {
        dist = meetRange;
        nearTarget = 1;
      }
    }
    if (dist > meetDist + footMove) {
      continue;
    }
    RangeMap_FillMoves(unit->x, unit->y, unit->type, footMove, 1);
    unitTileDist = NO_TILE;
    unitTileAlt = unitTileDist;
    if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
      RangeMap_SetBuffer(MAP_TILES->rangeScratch2);
      RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                         MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 0);
      RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
      if (MAP_TILES->rangeScratch2[MAP_TILE_INDEX(
              MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y)] < 0) {
        continue;
      }
    }
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
        if (!Ai_IsTransportStopTile(MAP_STATE->actingUnit->type, x, y)) {
          continue;
        }
        if (Map_IsSiloOrCapturableTile(x, y) == 1) {
          continue;
        }
        if (Unit_IsTileOwnedByTeam(MAP_STATE->actingUnitIndex,
                                   MAP_TILES->tiles[MAP_TILE_INDEX(x, y)]) ==
                1 &&
            gTerrainFactoryKind[MAP_TILE_TERRAIN(
                MAP_TILES->tiles[MAP_TILE_INDEX(x, y)])] != 0) {
          continue;
        }
        tileDist = MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)];
        if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
          tileAlt = MAP_TILES->rangeScratch2[MAP_TILE_INDEX(x, y)];
          if (tileAlt < 0) {
            tileAlt = NO_TILE;
          }
          if (tileDist < transportReach && unitTileDist < transportReach) {
            if (unitTileAlt < tileAlt) {
              continue;
            }
          } else if (tileDist > unitTileDist) {
            continue;
          }
        } else {
          tileAlt = NO_TILE;
          if (tileDist > unitTileDist) {
            continue;
          }
        }
        unitTileDist = tileDist;
        unitTileAlt = tileAlt;
        unitTileX = x;
        unitTileY = y;
      }
    }
    if (unitTileDist == NO_TILE) {
      continue;
    }
    meetX = unitTileX;
    meetY = unitTileY;
    meetSlot = i;
    if (nearTarget == 1) {
      if (meetDist > transportReach) {
        meetDist = transportReach;
      }
    } else {
      meetDist = MAP_TILES->rangeScratch[MAP_TILE_INDEX(unitTileX, unitTileY)];
    }
  }
  if (meetX == -1) {
    return;
  }
  gAiTransportsSentToUnit[meetSlot & 63]++;
  if (meetDist < transportReach) {
    AiUnit_PerformAction(meetX, meetY, AI_ACTION_WAIT, 0, 0);
  } else {
    AiUnit_MoveTowardTile(meetX, meetY);
  }
}
