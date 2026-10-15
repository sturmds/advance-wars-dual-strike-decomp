#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/range_map.h"
#include "game/terrain.h"
#include "game/ai/ai_tile_search.h"
#include "game/map_unit.h"
#include "game/math.h"
#include "game/ai/ai_unit_move.h"
#include "game/unit_stats.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_sea_units.h"
#include "game/co_data.h"
#include "game/ai/ai_unload.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_units_in_range.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define UNLIMITED_MOVE 110
/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define NO_TILE 0x7fff
/* Bit 7 of a AiSeaTransport_ScoreFootUnitsVsProperties score: set when Ai_TeamFootOrEnemyVehicleInRange returns 0. */
#define SCORE_FLAG 0x80
/* Added to a tile's score when the transport carries two units and both
 * can get off there. */
#define BOTH_CARGO_BONUS 0x100
/* MapUnit.aiOrder values copied between cargo units: 0-6. Orders 8-10 use
 * the unit's aiTarget; 7 has no routine in AiTransport_SecondCargoHasTaskAt. */
#define AI_ORDER_COPY_LIMIT 7
#define NO_TILE_POS 9999 /* an x or y no tile has: none found */
/* Default movement cost chart of the first CO record; -1 marks a terrain the move type cannot enter. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])
#define SCORE_CAN_REPAIR \
  0x10000 /* not a factory tile, and the unit is repaired here */
#define SCORE_MARKED_TILE \
  0x20000 /* Unit_IsBlackHoleOnFlaggedTile holds for the transport */
/* Sets count to the number of the four tiles next to (x, y) that a unit of
 * unitType could get off onto. */
#define COUNT_OPEN_NEIGHBOURS(count, x, y, unitType)           \
  count = AiTile_IsFreeForUnitType((x) - 1, (y), (unitType));  \
  count += AiTile_IsFreeForUnitType((x) + 1, (y), (unitType)); \
  count += AiTile_IsFreeForUnitType((x), (y) - 1, (unitType)); \
  count += AiTile_IsFreeForUnitType((x), (y) + 1, (unitType))

extern u8 gTerrainFactoryKind[];

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_FillUnitMoves(MapUnit* unit);
int MainLoop_GetFrameCount(void);
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
int Unit_GetTerrainStars(int player, int unitType, int terrain);
/* terrain defence stars */
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);

/* Whether the acting transport could unload its cargo at (x, y), as a count
 * of open tiles next to it; 0 means no. With one unit aboard, counts the
 * tiles open to that unit's type. With two, Tank tiles stand for any
 * vehicle and Infantry tiles for foot units: two vehicles count Tank tiles;
 * a vehicle in the second slot (first slot foot) falls back to 1 when only
 * Infantry tiles are open; a vehicle in the first slot (second slot foot)
 * needs two Tank tiles, else it falls back the same way; two foot units
 * count Infantry tiles. */
int AiTransport_CountUnloadTiles(int x, int y) {
  MapUnit* first;
  MapUnit* second;
  int count;

  second = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[1]];
  first = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]];
  if (second->type == UNIT_NONE) {
    COUNT_OPEN_NEIGHBOURS(count, x, y, first->type);
    return count;
  }
  if (second->type > UNIT_MECH) {
    if (first->type > UNIT_MECH) {
      COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_TANK);
      return count;
    }
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_TANK);
    if (count != 0) {
      return count;
    }
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_INFANTRY);
    return count != 0;
  }
  if (first->type > UNIT_MECH) {
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_TANK);
    if (count >= 2) {
      return count;
    }
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_INFANTRY);
    return count != 0;
  }
  COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_INFANTRY);
  return count;
}

/* How good (x, y) is as a tile to drop cargo unit `unit` on, for
 * AiTransport_FindDropTiles. -1: off the map, occupied, or terrain the
 * unit cannot enter. Otherwise higher is better: low threatSum counts
 * most, then a repair tile and a tile Unit_IsBlackHoleOnFlaggedTile
 * accepts add fixed bonuses, and terrain stars count unless the tile is a
 * factory tile the acting unit's team owns. The stars are read for
 * Infantry whatever the unit type. */
int AiTransport_ScoreDropTile(int x, int y, MapUnit* unit) {
  /* declaration order needed to match (register allocation) */
  int score;
  int tile;
  int threat;
  int terrain;
  int factory;

  score = 0;
  if (x < 0 || y < 0 || x >= MAP_STATE->unk_1c4->mapWidth ||
      y >= MAP_STATE->unk_1c4->mapHeight) {
    return -1;
  }
  tile = x + MAP_TILES->rowStart[y];
  if (MAP_TILES->unitAt[tile] != 0) {
    return -1;
  }
  terrain = MAP_TILE_TERRAIN(MAP_TILES->tiles[tile]);
  if (CHART[Ov000_gUnitStats[unit->type].moveType][terrain] < 0) {
    return -1;
  }
  factory = gTerrainFactoryKind[terrain];
  threat = MAP_TILES->threatSum[tile];
  if (factory == 0 || Unit_IsTileOwnedByTeam(MAP_STATE->actingUnitIndex,
                                             MAP_TILES->tiles[tile]) == 0) {
    score += (HwDiv32(Unit_GetTerrainStars(MAP_STATE->currentPlayer,
                                           UNIT_INFANTRY, terrain),
                      10) +
              1)
             << 4;
  }
  score += (256 - threat) << 8;
  if (gTerrainFactoryKind[terrain] == 0) {
    if (Unit_CanRepairAt(unit, x, y) == 1) {
      score += SCORE_CAN_REPAIR;
    }
  }
  if (Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y) == 1) {
    score += SCORE_MARKED_TILE;
  }
  return score;
}

/* Picks where the acting transport, standing at (x, y), unloads its cargo:
 * of the four tiles next to it, the one AiTransport_ScoreDropTile scores highest for
 * each cargo unit (a coin flip on the frame count breaks ties; the second
 * unit cannot take the first one's tile). Returns how many units got a
 * tile: 0, 1 or 2. */
int AiTransport_FindDropTiles(int x, int y, s16* firstDrop, s16* secondDrop) {
  /* declaration order and the shared cargo, score and best needed to
     * match (stack slots and registers); the "2" locals serve the second
     * cargo unit */
  int maxY2;
  int ty2;
  int maxX2;
  int minY2;
  int minX2;
  int tx2;
  int ty;
  int dist2;
  int maxY;
  int minY;
  int maxX;
  int best;
  int score;
  int tx;
  int dist;
  int minX;
  MapUnit* cargo;

  if (MAP_STATE->actingUnit->cargo[0] == 0) {
    return 0;
  }
  cargo = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]];
  best = 0;
  firstDrop[0] = NO_TILE_POS;
  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
  }
  for (ty = minY; ty <= maxY; ty++) {
    for (tx = minX; tx <= maxX; tx++) {
      dist = 0;
      if (x - tx > 0) {
        dist += x - tx;
      } else {
        dist += tx - x;
      }
      if (y - ty > 0) {
        dist += y - ty;
      } else {
        dist += ty - y;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      score = AiTransport_ScoreDropTile(tx, ty, cargo);
      if (score < best) {
        continue;
      }
      if (score == best && (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      best = score;
      firstDrop[0] = tx;
      firstDrop[1] = ty;
    }
  }
  if (firstDrop[0] == NO_TILE_POS) {
    return 0;
  }

  if (MAP_STATE->actingUnit->cargo[1] == 0) {
    return 1;
  }
  cargo = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[1]];
  best = 0;
  secondDrop[0] = NO_TILE_POS;
  minX2 = x - 1;
  if (minX2 < 0) {
    minX2 = 0;
  }
  minY2 = y - 1;
  if (minY2 < 0) {
    minY2 = 0;
  }
  maxX2 = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX2) {
    maxX2 = x + 1;
  }
  maxY2 = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY2) {
    maxY2 = y + 1;
  }
  for (ty2 = minY2; ty2 <= maxY2; ty2++) {
    for (tx2 = minX2; tx2 <= maxX2; tx2++) {
      dist2 = 0;
      if (x - tx2 > 0) {
        dist2 += x - tx2;
      } else {
        dist2 += tx2 - x;
      }
      if (y - ty2 > 0) {
        dist2 += y - ty2;
      } else {
        dist2 += ty2 - y;
      }
      if (dist2 < 1 || dist2 > 1) {
        continue;
      }
      if (tx2 == firstDrop[0] && ty2 == firstDrop[1]) {
        continue;
      }
      score = AiTransport_ScoreDropTile(tx2, ty2, cargo);
      if (score < best) {
        continue;
      }
      if (score == best && (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      best = score;
      secondDrop[0] = tx2;
      secondDrop[1] = ty2;
    }
  }
  return secondDrop[0] == NO_TILE_POS ? 1 : 2;
}

/* Queues the acting transport's unload at (x, y), on the tiles
 * AiTransport_FindDropTiles picks. When both cargo units have a drop tile
 * and the second has a task there, an unload with both directions is
 * queued first. When the second has no task there and neither unit is a
 * foot unit or an APC, the second takes the first one's order. The unload
 * of the first unit alone is queued in every case. */
void AiTransport_UnloadAt(int x, int y) {
  s16 firstDrop[2];
  s16 secondDrop[2];
  int dropCount;
  int firstDir;
  MapUnit* first;
  MapUnit* second;

  dropCount = AiTransport_FindDropTiles(x, y, firstDrop, secondDrop);
  if (dropCount == 0) {
    return;
  }
  firstDir = Tile_GetDirection(x, y, firstDrop[0], firstDrop[1]);
  if (dropCount >= 2) {
    if (AiTransport_SecondCargoHasTaskAt(x, y) == TRUE) {
      AiUnit_PerformAction(
          x, y, AI_ACTION_UNLOAD, firstDir,
          Tile_GetDirection(x, y, secondDrop[0], secondDrop[1]));
    } else {
      first = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]];
      if (first->type > UNIT_MECH && first->type != UNIT_APC &&
          first->aiOrder < AI_ORDER_COPY_LIMIT) {
        second = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[1]];
        if (second->type > UNIT_MECH && second->type != UNIT_APC &&
            second->aiOrder < AI_ORDER_COPY_LIMIT) {
          second->aiOrder = first->aiOrder;
        }
      }
    }
  }
  AiUnit_PerformAction(x, y, AI_ACTION_UNLOAD, firstDir, 0);
}

/* Run first by func_020b5bf4: drops the acting transport's cargo this
 * turn if it can. Of the free tiles in reach that have a nonzero
 * MapTiles.rangeScratch2 score and room to unload, takes the highest
 * score, then the lower threatMax + 2 * threatSum, and on a full tie
 * keeps the old tile on odd frames. Does nothing when no tile qualifies. */
void AiTransport_UnloadInReach(void) {
  /* declaration order needed to match (register allocation) */
  int count;
  int bestScore;
  int bestThreat;
  int score;
  int threat;
  int bestX;
  int bestY;
  int x;
  int y;

  bestX = -1;
  bestY = bestX;
  bestScore = 0;
  bestThreat = NO_TILE;
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] == 0) {
        continue;
      }
      count = AiTransport_CountUnloadTiles(x, y);
      if (count == 0) {
        continue;
      }
      score = (u8)MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)];
      if (MAP_STATE->actingUnit->cargo[1] != 0 && count >= 2) {
        score |= BOTH_CARGO_BONUS;
      }
      if (bestScore > score) {
        continue;
      }
      threat = MAP_STATE->mapTiles->threatMax[MAP_TILE_INDEX(x, y)] +
               MAP_STATE->mapTiles->threatSum[MAP_TILE_INDEX(x, y)] * 2;
      if (bestScore == score && bestThreat < threat) {
        continue;
      }
      if (bestScore == score && bestThreat == threat &&
          (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestScore = score;
      bestThreat = threat;
    }
  }
  if (bestScore == 0) {
    return;
  }
  AiTransport_UnloadAt(bestX, bestY);
}

/* Like AiTransport_HeadForUnloadTile, run before it by func_020b5bf4 when
 * the tile scores come from AiSeaTransport_ScoreFootUnitsVsProperties. A tile with SCORE_FLAG gets
 * one turn of slack: it beats an unflagged best up to one turn farther
 * away, and an unflagged tile beats a flagged best only when more than
 * one turn closer. Otherwise fewer turns, then the higher score, win. */
void AiTransport_HeadForFlaggedUnloadTile(void) {
  /* declaration order needed to match (register allocation) */
  int bestTurns;
  int bestX;
  int bestValue;
  int value;
  int x;
  int y;
  int bestY;
  int turns;

  bestX = -1;
  bestY = bestX;
  bestTurns = NO_TILE;
  bestValue = 0;
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->rangeMap[y][x] <= gAiTurnState->unk_05) {
        continue;
      }
      if (MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] == 0) {
        continue;
      }
      if (!AiTransport_CountUnloadTiles(x, y)) {
        continue;
      }
      turns = HwDiv32(MAP_STATE->rangeMap[y][x], gAiTurnState->unk_05) + 1;
      if (HwMod32(MAP_STATE->rangeMap[y][x], gAiTurnState->unk_05) == 0) {
        turns--;
      }
      value = (u8)MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)];
      if (bestTurns < turns) {
        if (bestValue & SCORE_FLAG) {
          continue;
        }
        if (!(value & SCORE_FLAG)) {
          continue;
        }
        if (bestTurns + 1 < turns) {
          continue;
        }
      }
      if (!(bestValue & SCORE_FLAG) && (value & SCORE_FLAG)) {
        if (bestTurns + 1 < turns) {
          continue;
        }
      } else if ((bestValue & SCORE_FLAG) && !(value & SCORE_FLAG)) {
        if (bestTurns - 1 <= turns) {
          continue;
        }
      } else {
        if (bestTurns < turns) {
          continue;
        }
        if (bestTurns == turns && bestValue > value) {
          continue;
        }
      }
      bestX = x;
      bestY = y;
      bestTurns = turns;
      bestValue = value;
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}

/* Run by func_020b5bf4 after AiTransport_UnloadInReach, which unloads the acting
 * transport on a tile in reach when it can. This one looks past this turn:
 * among the tiles with a nonzero MapTiles.rangeScratch2 value (a score
 * from code not read yet) that AiTransport_CountUnloadTiles accepts and that lie beyond
 * one turn's move (gAiTurnState->unk_05, likely the move per turn), it
 * takes the fewest turns away, then the highest score, and heads there
 * with AiUnit_MoveTowardTile. */
void AiTransport_HeadForUnloadTile(void) {
  /* declaration order needed to match (register allocation) */
  int bestTurns;
  int bestX;
  int bestValue;
  int value;
  int x;
  int y;
  int bestY;
  int turns;

  bestX = -1;
  bestY = bestX;
  bestTurns = NO_TILE;
  bestValue = 0;
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->rangeMap[y][x] <= gAiTurnState->unk_05) {
        continue;
      }
      if (MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] == 0) {
        continue;
      }
      if (!AiTransport_CountUnloadTiles(x, y)) {
        continue;
      }
      turns = HwDiv32(MAP_STATE->rangeMap[y][x], gAiTurnState->unk_05) + 1;
      if (HwMod32(MAP_STATE->rangeMap[y][x], gAiTurnState->unk_05) == 0) {
        turns--;
      }
      if (bestTurns < turns) {
        continue;
      }
      value = (u8)MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)];
      if (bestTurns == turns && bestValue > value) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestTurns = turns;
      bestValue = value;
    }
  }
  if (bestValue == 0) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}

/* Run by the sea transport routines when AI_STEP_UNLOAD_SCORED is set, after the
 * tile search. The score function foot unit transports use gets an extra
 * step (AiTransport_HeadForFlaggedUnloadTile). Purpose of the steps not known yet. */
void func_020b5bf4(void) {
  AiTransport_UnloadInReach();
  if (gAiUnloadScore == AiSeaTransport_ScoreFootUnitsVsProperties) {
    AiTransport_HeadForFlaggedUnloadTile();
  }
  AiTransport_HeadForUnloadTile();
}

/* TRUE when the acting unit has a target tile and the range map reaches it. */
BOOL AiUnit_HasReachableTarget(void) {
  MapUnit* unit = STATE->actingUnit;

  if (unit->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  return STATE->rangeMap[unit->aiTargetY][unit->aiTargetX] >= 0;
}

/* Writes the x and y of the first Shoal or Port tile (rows from the top) that
 * the range map reaches to out and returns TRUE; FALSE when there is none. */
BOOL AiSeaTransport_FindShoreTile(s16* out) {
  int x;
  int y;
  int width;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    width = MAP_STATE->unk_1c4->mapWidth;
    for (x = 0; x < width; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)) == TERRAIN_SHOAL ||
          MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)) == TERRAIN_PORT) {
        out[0] = x;
        out[1] = y;
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* The caller sets gAiUnloadTest (a test) and gAiUnloadScore (a score). For
 * each Shoal or Port tile the acting sea transport can reach, this fills
 * rangeMap with the first cargo unit's moves from that tile (in
 * MapTiles.rangeScratch) and runs the test. When it passes, every Shoal
 * and Port tile in the cargo's reach gets the score in
 * MapTiles.rangeScratch2 if higher, and AI_STEP_UNLOAD_SCORED is set. Those tiles
 * are then struck from the transport's own range map, so each shore group
 * is handled once. */
void AiSeaTransport_ScoreUnloadTiles(void) {
  /* declaration order needed to match (register allocation) */
  s16 shore[2];
  int cargoMove;
  int col;
  MapUnit* cargo;
  int x;
  int y;
  int row;
  int terrain;
  int score;

  cargo = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]];
  if (cargo->type == 0) {
    return;
  }
  cargoMove = AiUnit_GetUnloadMovement(cargo);
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 0);
  TileMap_Fill(MAP_TILES->rangeScratch2, 0);
  for (;;) {
    if (!AiSeaTransport_FindShoreTile(shore)) {
      return;
    }
    RangeMap_SetBuffer(MAP_TILES->rangeScratch);
    RangeMap_FillMoves(shore[0], shore[1], cargo->type, cargoMove, 0);
    if (gAiUnloadTest() == TRUE) {
      score = gAiUnloadScore();
      for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
          if (MAP_STATE->rangeMap[y][x] < 0) {
            continue;
          }
          terrain = MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(x, y)]);
          if (terrain == TERRAIN_SHOAL || terrain == TERRAIN_PORT) {
            if ((u8)MAP_TILES->rangeScratch2[MAP_TILE_INDEX(x, y)] < score) {
              MAP_TILES->rangeScratch2[MAP_TILE_INDEX(x, y)] = score;
              gAiTurnState->stepFlags |= AI_STEP_UNLOAD_SCORED;
            }
            MAP_TILES->rangeMapTiles[MAP_TILE_INDEX(x, y)] = -1;
          }
        }
      }
    } else {
      for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
        for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
          if (MAP_STATE->rangeMap[row][col] < 0) {
            continue;
          }
          terrain =
              MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(col, row)]);
          if (terrain == TERRAIN_SHOAL || terrain == TERRAIN_PORT) {
            MAP_TILES->rangeMapTiles[MAP_TILE_INDEX(col, row)] = -1;
          }
        }
      }
    }
    RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  }
}

/* Tile test for the foot unit and APC routine. FALSE when the acting unit has a
 * reachable target. Otherwise TRUE when the team's foot units reached number
 * less than a limit: the enemy property value reached plus 2 if
 * Ai_CountEnemyGroundUnitsInRange is nonzero, else Ai_ScoreReachableUnclaimedEnemyProperties's result
 * plus 1 (it must be nonzero). */
BOOL AiSeaTransport_FewFootUnitsNoTarget(void) {
  int useProperties;
  int value;
  int footUnits;

  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  useProperties = Ai_CountEnemyGroundUnitsInRange();
  if (useProperties) {
    value = Ai_ScoreReachableEnemyProperties();
  } else {
    value = Ai_ScoreReachableUnclaimedEnemyProperties();
  }
  if (value == 0) {
    return FALSE;
  }
  footUnits = Ai_CountTeamFootUnitsInRange();
  if (useProperties == 0) {
    if (footUnits < value + 1) {
      return TRUE;
    }
  } else if (footUnits < value + 2) {
    return TRUE;
  }
  return FALSE;
}

/* Score of a drop tile for a transport carrying foot units or an APC. Bits
 * 0-6: enemy property value reached plus 2, minus the team's foot units
 * reached (127 when that is negative or above 127). Bit 7: Ai_TeamFootOrEnemyVehicleInRange
 * (name not known yet) returns 0. Whether a higher score is better is not
 * known. */
int AiSeaTransport_ScoreFootUnitsVsProperties(void) {
  int properties = Ai_ScoreReachableEnemyProperties();
  int flag;
  int score;

  if (Ai_TeamFootOrEnemyVehicleInRange() == 0) {
    flag = 0x80;
  } else {
    flag = 0;
  }
  score = properties + 2 - Ai_CountTeamFootUnitsInRange();
  if (score & 0x80) {
    score = 0x7f;
  }
  return score | flag;
}
