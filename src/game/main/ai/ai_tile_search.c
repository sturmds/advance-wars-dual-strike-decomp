#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai/ai_best_damage.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/player.h"
#include "game/math.h"
#include "game/ai/ai_tile_search.h"
#include "game/ai.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define NO_TILE_POS 9999 /* an x or y no tile has: none found */
#define MAX_RANGE() \
  GetUnitBaseMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type)
#define SEARCH_RANGE_MAX 110
/* MapUnit.aiOrder value for which every tile is accepted. */
#define AI_ORDER_ANY_TILE 7
#define NO_TILE 0x7fff
#define HQ_COORD_MASK 0x7f
/* Score terms, added to 15 - rangeMap value (0 to 15). Threat counts most:
 * (768 - threat) << 9. Other terms are the terrain stars term and these. */
#define SCORE_FIREPOWER_TILE \
  0x100 /* armed unit, and TerrainFirepower gives a bonus here */
#define SCORE_CAN_REPAIR \
  0x80000 /* not a factory tile, and the unit is repaired here */
#define SCORE_MARKED_TILE \
  0x100000 /* Unit_IsBlackHoleOnFlaggedTile: a Black Hole unit on a tile with bit 7 of tileHp set */
#define SCORE_LOW_THREAT 0x200000 /* threat under 20 */

extern u8 gUnitSupplyKind[];
/* per unit type: 1 ground, 2 air, 3 sea */
extern u8 gTerrainIsProperty[];
/* per terrain id: 1 for properties (city, HQ, airport, port, base, ...) */
extern u8 gTerrainSupplyKind[];
/* per terrain id: kind of unit it supplies, 1 ground (city, HQ, base), 2 air, 3 sea */
extern u8 gTerrainFactoryKind[];

int UnitType_GetAttackKind(u8 unitType);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int GetUnitBaseMaxRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(s8 value);
int GetUnitMaxRange(int player, int unitType);
int Unit_GetUsableMovement(MapUnit* unit);
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
int GetUnitMovement(int player, int unitType);
int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);
BOOL Player_IsInGame(u8 player);
BOOL Player_HasHq(int player);
int MainLoop_GetFrameCount(void);
/* random number */
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
int Unit_GetTerrainStars(int player, int unitType, int terrain);
/* terrain defence stars */
int TerrainFirepower(int playerIndex, int unitType, int x, int y);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);

/* How good (x, y) is as a tile for the AI's acting unit to attack from.
 * -1: off the map, occupied, or out of reach (rangeMap above
 * gAiTurnState->unk_05, or negative). Otherwise higher is better. Threat
 * is MapTiles.threatMax + 2 * threatSum. Terrain stars count unless the
 * tile is a factory tile (base, airport, port) the unit's team owns. */
int AiTile_ScoreAttackFrom(int x, int y) {
  int reach;
  int score;
  int terrain;
  int threat;
  int tile;

  if (x < 0 || y < 0 || x >= MAP_STATE->unk_1c4->mapWidth ||
      y >= MAP_STATE->unk_1c4->mapHeight) {
    return -1;
  }
  tile = x + MAP_TILES->rowStart[y];
  if (MAP_TILES->unitAt[tile] != 0) {
    return -1;
  }
  reach = MAP_STATE->rangeMap[y][x];
  if (reach < 0 || reach > gAiTurnState->unk_05) {
    return -1;
  }
  threat = MAP_TILES->threatMax[tile] + MAP_TILES->threatSum[tile] * 2;
  terrain = MAP_TILE_TERRAIN(MAP_TILES->tiles[tile]);
  score = 15 - reach;
  if (score < 0) {
    score = 0;
  }
  if (gTerrainFactoryKind[terrain] == 0 ||
      Unit_IsTileOwnedByTeam(MAP_STATE->actingUnitIndex,
                             MAP_TILES->tiles[tile]) == 0) {
    score +=
        (HwDiv32(Unit_GetTerrainStars(MAP_STATE->currentPlayer,
                                      MAP_STATE->actingUnit->type, terrain),
                 10) +
         1)
        << 4;
  }
  if (TerrainFirepower(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type, x,
                       y) != 0) {
    if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange != 0) {
      score += SCORE_FIREPOWER_TILE;
    }
  }
  score += (768 - threat) << 9;
  if (gTerrainFactoryKind[terrain] == 0) {
    if (Unit_CanRepairAt(MAP_STATE->actingUnit, x, y) == 1) {
      score += SCORE_CAN_REPAIR;
    }
  }
  if (Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y) == 1) {
    score += SCORE_MARKED_TILE;
  }
  if (threat < 20) {
    score += SCORE_LOW_THREAT;
  }
  return score;
}

/* Picks the tile next to (targetX, targetY) that AiTile_ScoreAttackFrom scores
 * highest for the acting unit to attack from, and writes it to pos. Ties
 * go to a coin flip. FALSE: every side scores below 0. */
BOOL FindAttackTile(int targetX, int targetY, s16* pos) {
  int maxY;
  int maxX;
  int minX;
  int y;
  int bestScore;
  int score;
  int x;
  int dist;
  int minY;

  pos[0] = NO_TILE_POS;
  bestScore = 0;
  minX = targetX - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = targetY - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (targetX + 1 < maxX) {
    maxX = targetX + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (targetY + 1 < maxY) {
    maxY = targetY + 1;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (targetX - x > 0) {
        dist += targetX - x;
      } else {
        dist += x - targetX;
      }
      if (targetY - y > 0) {
        dist += targetY - y;
      } else {
        dist += y - targetY;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      score = AiTile_ScoreAttackFrom(x, y);
      if (score < bestScore) {
        continue;
      }
      if (score == bestScore && (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      pos[0] = x;
      bestScore = score;
      pos[1] = y;
    }
  }
  return pos[0] != NO_TILE_POS;
}

/* Name is a guess. Like FindAttackTile, but a side must also pass Ai_IsTransportStopTile for the
 * acting unit's type. Writes the best side to pos; ties go to a coin flip.
 * FALSE: no side qualifies. */
BOOL AiUnit_FindStandTileNextTo(int targetX, int targetY, s16* pos) {
  int maxY;
  int maxX;
  int minX;
  int y;
  int bestScore;
  int score;
  int x;
  int dist;
  int minY;

  pos[0] = NO_TILE_POS;
  bestScore = 0;
  minX = targetX - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = targetY - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (targetX + 1 < maxX) {
    maxX = targetX + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (targetY + 1 < maxY) {
    maxY = targetY + 1;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (targetX - x > 0) {
        dist += targetX - x;
      } else {
        dist += x - targetX;
      }
      if (targetY - y > 0) {
        dist += targetY - y;
      } else {
        dist += y - targetY;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      if (!Ai_IsTransportStopTile(MAP_STATE->actingUnit->type, x, y)) {
        continue;
      }
      score = AiTile_ScoreAttackFrom(x, y);
      if (score < bestScore) {
        continue;
      }
      if (score == bestScore && (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      pos[0] = x;
      bestScore = score;
      pos[1] = y;
    }
  }
  return pos[0] != NO_TILE_POS;
}

/* Finds the HQ of an opposing player in game, among those with a
 * non-negative value in the range map, that has the lowest range map
 * value (the last one on a tie), and writes its x and y to out. FALSE when
 * there is none. */
BOOL Ai_FindNearestEnemyHqTile(s16* out) {
  int best;
  int player;
  int x;
  int y;
  s8 value;

  best = NO_TILE;
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
        MAP_STATE->players[player].team) {
      continue;
    }
    if (!Player_HasHq(player)) {
      continue;
    }
    x = MAP_STATE->players[player].hqX & HQ_COORD_MASK;
    y = MAP_STATE->players[player].hqY & HQ_COORD_MASK;
    value = MAP_STATE->rangeMap[y][x];
    if (value < 0) {
      continue;
    }
    if (value <= best) {
      out[0] = x;
      best = value;
      out[1] = y;
    }
  }
  return best != NO_TILE;
}

/* AiUnit_MoveToSafestTile and other AI move pickers skip (x, y) when this
 * returns 1. It does so when:
 * - turn->domainsOwn has AI_DOMAIN_SEA and a unit of the current player's team with
 *   MapUnit.transportWanted == 2 stands on a shoal or port next to (x, y), or
 * - an enemy next to (x, y) has exactly one free side the acting unit can
 *   reach, and an own direct-attack unit other than Infantry or Mech, in the
 *   acting unit's move group (ground or sea), stands within 4 tiles. That
 *   unit must not have waited, nor have MapUnit.unk_08_7 set unless (x, y)
 *   is the acting unit's own tile.
 * With no own unit next to it, a tile with hp counts as an enemy when
 * AI_TARGET_FLAGS->attackTiles is off. */
int Ai_IsTileNeededByAlly(s16 x, s16 y) {
  /* needed to match: declaration order sets registers and stack slots */
  int freeMaxY;
  int farMaxY;
  int farMaxX;
  int farMinX;
  int sideMaxY;
  int sideMaxX;
  int sideY;
  int sideMinY;
  int sideMinX;
  int shoreMaxY;
  int shoreMinX;
  int sideX;
  AiTurnState* turn;
  int shoreMaxX;
  int shoreY;
  int dist;
  int terrain;
  int shoreMinY;
  int shoreX;
  int slot;
  s16 enemyX;
  int freeMinX;
  int freeMaxX;
  int freeMinY;
  s16 enemyY;
  int count; /* free sides of an enemy, then the result */
  int freeX;
  int freeY;
  int farY;
  MapUnit* unit;
  int farX;

  turn = gAiTurnState;
  if (turn->domainsOwn & AI_DOMAIN_SEA) {
    shoreMinX = x - 1;
    if (shoreMinX < 0) {
      shoreMinX = 0;
    }
    shoreMinY = y - 1;
    if (shoreMinY < 0) {
      shoreMinY = 0;
    }
    shoreMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
    if (x + 1 < shoreMaxX) {
      shoreMaxX = x + 1;
    }
    shoreMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
    if (y + 1 < shoreMaxY) {
      shoreMaxY = y + 1;
    }
    for (shoreY = shoreMinY; shoreY <= shoreMaxY; shoreY++) {
      for (shoreX = shoreMinX; shoreX <= shoreMaxX; shoreX++) {
        dist = 0;
        if (x - shoreX > 0) {
          dist += x - shoreX;
        } else {
          dist += shoreX - x;
        }
        if (y - shoreY > 0) {
          dist += y - shoreY;
        } else {
          dist += shoreY - y;
        }
        if (dist < 1 || dist > 1) {
          continue;
        }
        terrain = MAP_TILE_TERRAIN(
            MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(shoreX, shoreY)]);
        if (terrain == TERRAIN_SHOAL || terrain == TERRAIN_PORT) {
          slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(shoreX, shoreY)];
          if (slot == 0) {
            continue;
          }
          if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
              MAP_STATE->players[(slot >> 6) + 1].team) {
            continue;
          }
          if (MAP_STATE->units[slot].transportWanted == 2) {
            return 1;
          }
        }
      }
    }
  }

  /* look for an enemy with one free side left */
  count = 0;
  sideMinX = x - 1;
  if (sideMinX < 0) {
    sideMinX = 0;
  }
  sideMinY = y - 1;
  if (sideMinY < 0) {
    sideMinY = 0;
  }
  sideMaxX = x + 1;
  farMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (sideMaxX >= farMaxX) {
    sideMaxX = farMaxX;
  }
  sideMaxY = y + 1;
  farMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (sideMaxY >= farMaxY) {
    sideMaxY = farMaxY;
  }
  for (sideY = sideMinY; sideY <= sideMaxY; sideY++) {
    for (sideX = sideMinX; sideX <= sideMaxX; sideX++) {
      dist = 0;
      if (x - sideX > 0) {
        dist += x - sideX;
      } else {
        dist += sideX - x;
      }
      if (y - sideY > 0) {
        dist += y - sideY;
      } else {
        dist += sideY - y;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(sideX, sideY)];
      if (slot == 0) {
        if (AI_TARGET_FLAGS->attackTiles) {
          continue;
        }
        if ((MAP_STATE->tileHp[MAP_TILE_INDEX(sideX, sideY)] & 0x7f) == 0) {
          continue;
        }
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      /* count the free sides of the enemy the acting unit can reach */
      enemyX = sideX;
      enemyY = sideY;
      freeMinX = enemyX - 1;
      if (freeMinX < 0) {
        freeMinX = 0;
      }
      freeMinY = enemyY - 1;
      if (freeMinY < 0) {
        freeMinY = 0;
      }
      freeMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
      if (enemyX + 1 < freeMaxX) {
        freeMaxX = enemyX + 1;
      }
      freeMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
      if (enemyY + 1 < freeMaxY) {
        freeMaxY = enemyY + 1;
      }
      for (freeY = freeMinY; freeY <= freeMaxY; freeY++) {
        for (freeX = freeMinX; freeX <= freeMaxX; freeX++) {
          dist = 0;
          if (enemyX - freeX > 0) {
            dist += enemyX - freeX;
          } else {
            dist += freeX - enemyX;
          }
          if (enemyY - freeY > 0) {
            dist += enemyY - freeY;
          } else {
            dist += freeY - enemyY;
          }
          if (dist < 1 || dist > 1) {
            continue;
          }
          if (MAP_STATE->rangeMap[freeY][freeX] < 0) {
            continue;
          }
          if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(freeX, freeY)] == 0) {
            count++;
          }
        }
      }
      if (count == 1) {
        goto counted;
      }
      count = 0;
    }
  }
counted:
  if (count == 0) {
    return 0;
  }

  /* look for an own unit that could attack it */
  count = 0;
  farMinX = x - 4;
  if (farMinX < 0) {
    farMinX = 0;
  }
  farY = y - 4;
  if (farY < 0) {
    farY = 0;
  }
  if (x + 4 < farMaxX) {
    farMaxX = x + 4;
  }
  if (y + 4 < farMaxY) {
    farMaxY = y + 4;
  }
  for (; farY <= farMaxY; farY++) {
    for (farX = farMinX; farX <= farMaxX; farX++) {
      dist = 0;
      if (x - farX > 0) {
        dist += x - farX;
      } else {
        dist += farX - x;
      }
      if (y - farY > 0) {
        dist += y - farY;
      } else {
        dist += farY - y;
      }
      if (dist < 1 || dist > 4) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(farX, farY)];
      if (slot == 0) {
        continue;
      }
      if (farX == MAP_STATE->actingUnit->x &&
          farY == MAP_STATE->actingUnit->y) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      if (unit->flags & MAP_UNIT_WAITED) {
        continue;
      }
      if (!(x == MAP_STATE->actingUnit->x && y == MAP_STATE->actingUnit->y) &&
          unit->unk_08_7) {
        continue;
      }
      if (UnitType_GetAttackKind(unit->type) != ATTACK_KIND_DIRECT) {
        continue;
      }
      if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
          (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
        if (!(Ov000_gUnitStats[unit->type].moveGroup &
              (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
          continue;
        }
      } else if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
                 MOVE_GROUP_SEA) {
        if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_SEA)) {
          continue;
        }
      } else {
        continue;
      }
      if (unit->type > UNIT_MECH) {
        count = 1;
        goto done;
      }
    }
  }
done:
  return count ? 1 : 0;
}

/* Whether the AI's acting unit may end its move on (x, y), used by the
 * attack planners for the tile they attack from. Occupied tiles are
 * refused; empty tiles that are not properties are fine. On a property
 * its team does not own,
 * a unit other than Infantry and Mech keeps off when a friendly Infantry
 * or Mech that has not moved yet is within 3 tiles (it could capture),
 * and keeps off a port when a friendly Lander or Black Boat that has not
 * moved is within a Lander's move. On its own team's base, airport or
 * port it keeps off unless the property supplies its kind of unit and
 * bit 1 of gAiTurnState->flags is set, or it is a Lander with one of the
 * player's own bases within 6 tiles, or a Black Boat with one within 3.
 * A player's own port is also kept free while it can afford a
 * Battleship. Each search has its own variables and the ship search uses
 * ternaries: both needed to match. */
BOOL Ai_CanEndMoveAt(s16 x, s16 y) {
  /* needed to match: declaration order sets registers and stack slots */
  int shipMaxY;
  int shipMaxX;
  int boatMaxY;
  int slot;
  int boatMaxX;
  int boatY;
  int boatMinY;
  int maxY;
  int boatMinX;
  int shipY;
  int landerMinX;
  MapUnit* unit;
  int landerMaxY;
  int landerMaxX;
  int maxX;
  int cy;
  int cx;
  int landerY;
  int minX;
  int landerMinY;
  int shipMinX;
  int minY;
  int shipX;
  int distance;
  int boatX;
  int landerX;

  if (MAP_STATE->actingUnit->aiOrder == AI_ORDER_ANY_TILE) {
    return TRUE;
  }
  if (MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
    return FALSE;
  }
  if (gTerrainIsProperty[MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))] == 0) {
    return TRUE;
  }
  /* needed to match: the s16 cast gives ldrsh */
  if (Unit_IsTileOwnedByTeam((s16)MAP_STATE->currentUnitBase,
                             MAP_TILE_AT(x, y)) == 0) {
    if (MAP_STATE->actingUnit->type <= UNIT_MECH) {
      return TRUE;
    }
    /* A friendly Infantry or Mech that has not moved may want to capture. */
    minX = x - 3;
    if (minX < 0) {
      minX = 0;
    }
    minY = y - 3;
    if (minY < 0) {
      minY = 0;
    }
    maxX = MAP_STATE->unk_1c4->mapWidth - 1;
    if (x + 3 < maxX) {
      maxX = x + 3;
    }
    maxY = MAP_STATE->unk_1c4->mapHeight - 1;
    if (y + 3 < maxY) {
      maxY = y + 3;
    }
    for (cy = minY; cy <= maxY; cy++) {
      for (cx = minX; cx <= maxX; cx++) {
        distance = 0;
        if (x - cx > 0) {
          distance += x - cx;
        } else {
          distance += cx - x;
        }
        if (y - cy > 0) {
          distance += y - cy;
        } else {
          distance += cy - y;
        }
        if (distance < 1 || distance > 3) {
          continue;
        }
        slot = MAP_TILES->unitAt[MAP_TILE_INDEX(cx, cy)];
        unit = &MAP_STATE->units[slot];
        if (unit->type == UNIT_NONE) {
          continue;
        }
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[(slot >> 6) + 1].team) {
          continue;
        }
        if (unit->type > UNIT_MECH) {
          continue;
        }
        if (!(unit->flags & MAP_UNIT_WAITED)) {
          return FALSE;
        }
      }
    }
    /* A friendly Lander or Black Boat that has not moved may need this port. */
    if (MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)) == TERRAIN_PORT) {
      if (x - GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER) < 0) {
        shipMinX = 0;
      } else {
        shipMinX = x - GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER);
      }
      if (y - GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER) < 0) {
        shipY = 0;
      } else {
        shipY = y - GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER);
      }
      shipMaxX =
          x + GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER) <
                  MAP_STATE->unk_1c4->mapWidth - 1
              ? x + GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER)
              : MAP_STATE->unk_1c4->mapWidth - 1;
      shipMaxY =
          y + GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER) <
                  MAP_STATE->unk_1c4->mapHeight - 1
              ? y + GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER)
              : MAP_STATE->unk_1c4->mapHeight - 1;
      /* shipY starts at the lowest row: needed to match */
      for (; shipY <= shipMaxY; shipY++) {
        for (shipX = shipMinX; shipX <= shipMaxX; shipX++) {
          distance = 0;
          if (x - shipX > 0) {
            distance += x - shipX;
          } else {
            distance += shipX - x;
          }
          if (y - shipY > 0) {
            distance += y - shipY;
          } else {
            distance += shipY - y;
          }
          if (distance < 1 ||
              distance >
                  GetUnitMovement(MAP_STATE->currentPlayer, UNIT_LANDER)) {
            continue;
          }
          slot = MAP_TILES->unitAt[MAP_TILE_INDEX(shipX, shipY)];
          unit = &MAP_STATE->units[slot];
          if (unit->type == UNIT_NONE) {
            continue;
          }
          if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
              MAP_STATE->players[(slot >> 6) + 1].team) {
            continue;
          }
          if (unit->type != UNIT_LANDER && unit->type != UNIT_BLACK_BOAT) {
            continue;
          }
          if (!(unit->flags & MAP_UNIT_WAITED)) {
            return FALSE;
          }
        }
      }
    }
    return TRUE;
  }

  if (gTerrainFactoryKind[MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))] == 0) {
    return TRUE;
  }
  if (gUnitSupplyKind[MAP_STATE->actingUnit->type] ==
          gTerrainSupplyKind[MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))] &&
      AI_TARGET_FLAGS->unk_1) {
    return TRUE;
  }
  /* Keep the player's own port free while it can afford a Battleship.
     * needed to match: the u32 cast makes the compare unsigned */
  if (MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)) == TERRAIN_PORT &&
      MAP_STATE->unk_04 == (MAP_TILE_AT(x, y) & 0xe0)) {
    if ((u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_BATTLESHIP) *
              10) <= MAP_STATE->players[MAP_STATE->currentPlayer].funds) {
      return FALSE;
    }
  }
  if (MAP_STATE->actingUnit->type == UNIT_LANDER) {
    landerMinX = x - 6;
    if (landerMinX < 0) {
      landerMinX = 0;
    }
    landerMinY = y - 6;
    if (landerMinY < 0) {
      landerMinY = 0;
    }
    landerMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
    if (x + 6 < landerMaxX) {
      landerMaxX = x + 6;
    }
    landerMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
    if (y + 6 < landerMaxY) {
      landerMaxY = y + 6;
    }
    for (landerY = landerMinY; landerY <= landerMaxY; landerY++) {
      for (landerX = landerMinX; landerX <= landerMaxX; landerX++) {
        distance = 0;
        if (x - landerX > 0) {
          distance += x - landerX;
        } else {
          distance += landerX - x;
        }
        if (y - landerY > 0) {
          distance += y - landerY;
        } else {
          distance += landerY - y;
        }
        if (distance < 1 || distance > 6) {
          continue;
        }
        if (MAP_TILE_TERRAIN(MAP_TILE_AT(landerX, landerY)) == TERRAIN_BASE &&
            MAP_STATE->unk_04 == (MAP_TILE_AT(landerX, landerY) & 0xe0)) {
          return TRUE;
        }
      }
    }
  }
  if (MAP_STATE->actingUnit->type == UNIT_BLACK_BOAT) {
    boatMinX = x - 3;
    if (boatMinX < 0) {
      boatMinX = 0;
    }
    boatMinY = y - 3;
    if (boatMinY < 0) {
      boatMinY = 0;
    }
    boatMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
    if (x + 3 < boatMaxX) {
      boatMaxX = x + 3;
    }
    boatMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
    if (y + 3 < boatMaxY) {
      boatMaxY = y + 3;
    }
    for (boatY = boatMinY; boatY <= boatMaxY; boatY++) {
      for (boatX = boatMinX; boatX <= boatMaxX; boatX++) {
        distance = 0;
        if (x - boatX > 0) {
          distance += x - boatX;
        } else {
          distance += boatX - x;
        }
        if (y - boatY > 0) {
          distance += y - boatY;
        } else {
          distance += boatY - y;
        }
        if (distance < 1 || distance > 3) {
          continue;
        }
        if (MAP_TILE_TERRAIN(MAP_TILE_AT(boatX, boatY)) == TERRAIN_BASE &&
            MAP_STATE->unk_04 == (MAP_TILE_AT(boatX, boatY) & 0xe0)) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

/* Movement budget the AI gives the move-range fill for a unit. 110 (no limit
 * in practice) when the player has no sea domain, the unit is not a ground
 * unit, or its claimedLandmass flag is set. Otherwise a foot unit counts 4, other
 * ground units their usable movement, scaled by a gAiProfile factor picked by
 * unk_0c_4, capped at 110. The purpose is a guess: callers use it for capture
 * targets, and probably allow for ferrying by sea. */
int AiUnit_GetSearchMovement(MapUnit* unit) {
  int range;

  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return SEARCH_RANGE_MAX;
  }
  if (!(Ov000_gUnitStats[unit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
    return SEARCH_RANGE_MAX;
  }
  if (unit->claimedLandmass) {
    return SEARCH_RANGE_MAX;
  }
  if (unit->type <= UNIT_MECH) {
    range = 4;
  } else {
    range = Unit_GetUsableMovement(unit);
  }
  if (unit->unk_0c_4) {
    range = range * gAiProfile->unk_08[7];
  } else {
    range = range * gAiProfile->unk_08[6];
  }
  if (range > SEARCH_RANGE_MAX) {
    range = SEARCH_RANGE_MAX;
  }
  return range;
}

/* Same as AiUnit_GetSearchMovement, for the acting unit. A unit that is
 * not a foot unit counts its moves this turn (gAiTurnState->unk_05). The
 * name is a guess too. */
int AiActingUnit_GetSearchMovement(void) {
  MapUnit* unit;
  int range;

  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return SEARCH_RANGE_MAX;
  }
  unit = MAP_STATE->actingUnit;
  if (!(Ov000_gUnitStats[unit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
    return SEARCH_RANGE_MAX;
  }
  if (unit->claimedLandmass) {
    return SEARCH_RANGE_MAX;
  }
  if (unit->type <= UNIT_MECH) {
    range = 4;
  } else {
    range = gAiTurnState->unk_05;
  }
  if (unit->unk_0c_4) {
    range = range * gAiProfile->unk_08[7];
  } else {
    range = range * gAiProfile->unk_08[6];
  }
  if (range > SEARCH_RANGE_MAX) {
    range = SEARCH_RANGE_MAX;
  }
  return range;
}

/* Three quarters of the search movement AiActingUnit_GetSearchMovement
 * gives for the unit, which is made the acting unit for the call. An empty
 * slot (type 0) gets the maximum. */
int AiUnit_GetUnloadMovement(MapUnit* unit) {
  MapUnit* previous = MAP_STATE->actingUnit;
  int movement;

  if (unit->type == UNIT_NONE) {
    return SEARCH_RANGE_MAX;
  }
  MAP_STATE->actingUnit = unit;
  movement = AiActingUnit_GetSearchMovement();
  MAP_STATE->actingUnit = previous;
  return movement * 3 >> 2;
}

/* Fills the range map with the acting unit's moves, then marks the tiles
 * around the reachable ones for each step of its attack range, with values
 * 111 and up. Indirect units use the CO-adjusted maximum range; the others
 * use minRange, which is 1 for a direct unit. */
void AiActingUnit_MarkAttackRange(int movement) {
  int i;
  int range;

  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, movement, 0);
  if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) ==
      ATTACK_KIND_INDIRECT) {
    range =
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  } else {
    range = Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange;
  }
  for (i = 0; i < range; i++) {
    RangeMap_MarkAdjacentTiles(111 + i);
  }
}

/* For an indirect unit about to attack the tile in pos: replaces pos with
 * the reachable tile, 0 to 110 move cost, that is in the unit's firing range
 * of it, nearest first and then cheapest to reach. Returns FALSE when no
 * tile qualifies. Other unit kinds keep pos and return TRUE. */
BOOL AiUnit_FindFiringTile(s16* pos) {
  int xMin;
  int cost;
  int bestX;
  int bestDist;
  int y;
  int xMax;
  int yMax;
  int x;
  int dist;
  int bestY;

  if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) !=
      ATTACK_KIND_INDIRECT) {
    return TRUE;
  }
  bestY = NO_TILE_POS;
  bestX = NO_TILE_POS;
  bestDist = MAX_RANGE() + 1;
  xMin = pos[0] - MAX_RANGE() < 0 ? 0 : pos[0] - MAX_RANGE();
  y = pos[1] - MAX_RANGE() < 0 ? 0 : pos[1] - MAX_RANGE();
  xMax = pos[0] + MAX_RANGE() < MAP_STATE->unk_1c4->mapWidth - 1
             ? pos[0] + MAX_RANGE()
             : MAP_STATE->unk_1c4->mapWidth - 1;
  yMax = pos[1] + MAX_RANGE() < MAP_STATE->unk_1c4->mapHeight - 1
             ? pos[1] + MAX_RANGE()
             : MAP_STATE->unk_1c4->mapHeight - 1;
  for (; y <= yMax; y++) {
    for (x = xMin; x <= xMax; x++) {
      dist = 0;
      if (pos[0] - x > 0) {
        dist += pos[0] - x;
      } else {
        dist += x - pos[0];
      }
      if (pos[1] - y > 0) {
        dist += pos[1] - y;
      } else {
        dist += y - pos[1];
      }
      if (dist < Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange) {
        continue;
      }
      if (dist > MAX_RANGE() || dist > bestDist) {
        continue;
      }
      cost = MAP_STATE->rangeMap[y][x];
      if (cost < 0 || cost > 110) {
        continue;
      }
      if (bestX != NO_TILE_POS && dist == bestDist &&
          cost > MAP_STATE->rangeMap[bestY][bestX]) {
        continue;
      }
      bestDist = dist;
      bestX = x;
      bestY = y;
    }
  }
  if (bestX == NO_TILE_POS) {
    return FALSE;
  }
  pos[0] = bestX;
  pos[1] = bestY;
  return TRUE;
}

/* TRUE when the acting direct-attack unit's hit on the target is weak (the
 * simulated battle in combatants costs the target under 30 hp) and a
 * ground teammate that has not acted yet, within 4 tiles of the target,
 * would do clearly more damage (hp times damage chart value, by more than
 * 2000). The callers then look for an attack position that leaves such a
 * teammate room. */
int AiUnit_TeammateOutdamages(MapUnit* target) {
  /* needed to match: declaration order, the two clamp forms below and the
     * row loop starting from y itself set the registers and stack slots */
  u8 slot;
  int y;
  int maxY;
  int maxX;
  MapUnit* other;
  int dist;
  int x;
  int minX;

  if (!(Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD |
         MOVE_GROUP_SEA))) {
    return 0;
  }
  if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) !=
      ATTACK_KIND_DIRECT) {
    return 0;
  }
  if (MAP_STATE->combatants[1].hp == 0) {
    return 0;
  }
  if (MAP_STATE->combatants[1].hpLost < 30) {
    minX = target->x - 4;
    if (minX < 0) {
      minX = 0;
    }
    if (target->y - 4 < 0) {
      y = 0;
    } else {
      y = target->y - 4;
    }
    maxX = MAP_STATE->unk_1c4->mapWidth - 1;
    if (target->x + 4 < maxX) {
      maxX = target->x + 4;
    }
    maxY = MAP_STATE->unk_1c4->mapHeight - 1;
    if (target->y + 4 < maxY) {
      maxY = target->y + 4;
    }
    for (; y <= maxY; y++) {
      for (x = minX; x <= maxX; x++) {
        dist = 0;
        if (target->x - x > 0) {
          dist += target->x - x;
        } else {
          dist += x - target->x;
        }
        if (target->y - y > 0) {
          dist += target->y - y;
        } else {
          dist += y - target->y;
        }
        if (dist < 0 || dist > 4) {
          continue;
        }
        slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
        if (slot == 0 || !Unit_IsCurrentPlayers(slot)) {
          continue;
        }
        if (x == MAP_STATE->actingUnit->x && y == MAP_STATE->actingUnit->y) {
          continue;
        }
        other = &MAP_STATE->units[slot];
        if (UnitType_GetAttackKind(other->type) == ATTACK_KIND_INDIRECT) {
          continue;
        }
        if (Ov000_gUnitStats[other->type].moveGroup & MOVE_GROUP_AIR) {
          continue;
        }
        if (other->flags & MAP_UNIT_WAITED) {
          continue;
        }
        if (other->type == UNIT_NONE) {
          continue;
        }
        if (other->captureProgress != 0) {
          continue;
        }
        if (MAP_STATE->actingUnit->hp *
                    Ai_BestDamage(MAP_STATE->actingUnit->type, target->type,
                                  0) +
                2000 <
            other->hp * Ai_BestDamage(other->type, target->type, 0)) {
          return 1;
        }
      }
    }
  }
  return 0;
}
