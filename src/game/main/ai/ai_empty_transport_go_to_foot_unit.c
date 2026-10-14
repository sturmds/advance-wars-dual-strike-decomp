#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gTerrainFactoryKind
    []; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */
extern u8 gAiTransportsSentToUnit
    []; /* per unit slot & 63: empty transports already sent to that foot unit this turn */

/* movement is int here (s16 in fill_unit_moves.c): needed to match, the
 * caller passes footMove without narrowing it. */
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Player_GetUnitMoveRange(int player, int unitType);
int Unit_GetUsableMovement(
    MapUnit* unit); /* move points this turn, capped by fuel */
int func_020a507c(int x, int y, int range);
int func_020a50d4(int x, int y, int unitType, int movement);
BOOL Ai_IsTransportStopTile(int unitType, int x, int y);
BOOL Map_IsSiloOrCapturableTile(int x, int y);
int Unit_IsTileOwnedByTeam(int unitIndex,
                           int tile); /* 1: tile owned by the unit's team */
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1)

/* Movement given to range fills that should cover the whole map. */
#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff

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

  if (STATE->actingUnit->cargo[0] != 0) {
    return;
  }
  RangeMap_SetBuffer(MAP->rangeScratch);
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  RangeMap_SetBuffer(MAP->rangeMapTiles);
  transportReach = Unit_GetUsableMovement(STATE->actingUnit) + 1;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &STATE->units[i];
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
    switch (STATE->actingUnit->type) {
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
    footMove = Player_GetUnitMoveRange(STATE->currentPlayer, unit->type);
    dist = MAP->rangeScratch[TILE_INDEX(unit->x, unit->y)];
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
      RangeMap_SetBuffer(MAP->rangeScratch2);
      RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                         STATE->actingUnit->type, UNLIMITED_MOVE, 0);
      RangeMap_SetBuffer(MAP->rangeMapTiles);
      if (MAP->rangeScratch2[TILE_INDEX(STATE->actingUnit->x,
                                        STATE->actingUnit->y)] < 0) {
        continue;
      }
    }
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
        if (STATE->rangeMap[y][x] < 0) {
          continue;
        }
        if (MAP->rangeScratch[TILE_INDEX(x, y)] < 0) {
          continue;
        }
        if (MAP->unitAt[TILE_INDEX(x, y)] != 0) {
          continue;
        }
        if (!Ai_IsTransportStopTile(STATE->actingUnit->type, x, y)) {
          continue;
        }
        if (Map_IsSiloOrCapturableTile(x, y) == 1) {
          continue;
        }
        if (Unit_IsTileOwnedByTeam(STATE->actingUnitIndex,
                                   MAP->tiles[TILE_INDEX(x, y)]) == 1 &&
            gTerrainFactoryKind[MAP_TILE_TERRAIN(
                MAP->tiles[TILE_INDEX(x, y)])] != 0) {
          continue;
        }
        tileDist = MAP->rangeScratch[TILE_INDEX(x, y)];
        if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
          tileAlt = MAP->rangeScratch2[TILE_INDEX(x, y)];
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
      meetDist = MAP->rangeScratch[TILE_INDEX(unitTileX, unitTileY)];
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
