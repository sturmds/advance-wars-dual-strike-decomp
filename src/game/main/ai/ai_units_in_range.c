#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/ai/ai_units_in_range.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

#define MOVE_GROUP_GROUND \
  (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)

extern u8 gTerrainIsProperty[];

BOOL Player_IsInGame(u8 player);

/* per terrain id: 1 for properties */
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);

/* Value of the properties the acting unit can reach: 1 for each property
 * on a reached tile that its team does not own, 30 for an HQ. */
int Ai_ScoreReachableEnemyProperties(void) {
  int score = 0;
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!gTerrainIsProperty[MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))]) {
        continue;
      }
      if (Unit_IsTileOwnedByTeam(MAP_STATE->actingUnitIndex,
                                 MAP_TILE_AT(x, y))) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)) == TERRAIN_HQ) {
        score += 30;
      } else {
        score += 1;
      }
    }
  }
  return score;
}

/* Like Ai_ScoreReachableEnemyProperties, but a property that AI units are
 * already heading for to capture counts only if it is an HQ: 1 for each
 * other property on a reached tile that the acting unit's team does not own,
 * 30 for an HQ. */
int Ai_ScoreReachableUnclaimedEnemyProperties(void) {
  int score = 0;
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!gTerrainIsProperty[MAP_TILE_TERRAIN(
              MAP_TILES->tiles[MAP_TILE_INDEX(x, y)])]) {
        continue;
      }
      if (Unit_IsTileOwnedByTeam(MAP_STATE->actingUnitIndex,
                                 MAP_TILES->tiles[MAP_TILE_INDEX(x, y)])) {
        continue;
      }
      if (MAP_STATE->unk_6088[MAP_TILES->unk_1928[MAP_TILE_INDEX(x, y)]]
                  .claimCount != 0 &&
          MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(x, y)]) !=
              TERRAIN_HQ) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(x, y)]) ==
          TERRAIN_HQ) {
        score += 30;
      } else {
        score += 1;
      }
    }
  }
  return score;
}

/* Counts the Infantry and Mechs of the current player's team (all players
 * in the game with its team) that stand on a tile the range map reaches.
 * Units being carried are skipped. Only slots 1-50 of each player count. */
int Ai_CountTeamFootUnitsInRange(void) {
  int count = 0;
  int player;
  int slot;
  MapUnit* unit;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
        MAP_STATE->players[player].team) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &MAP_STATE->units[slot];
      if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED)) {
        continue;
      }
      if (unit->type > UNIT_MECH) {
        continue;
      }
      if (MAP_STATE->rangeMap[unit->y][unit->x] != -1) {
        count++;
      }
    }
  }
  return count;
}

/* Counts the armed ground units other than Infantry and Mech of the current player's team (all players
 * in the game with its team) that stand on a tile the range map reaches.
 * Units being carried are skipped. Only slots 1-50 of each player count. */
int Ai_CountTeamArmedGroundUnitsInRange(void) {
  int count = 0;
  int player;
  int slot;
  MapUnit* unit;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
        MAP_STATE->players[player].team) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &MAP_STATE->units[slot];
      if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED)) {
        continue;
      }
      if (unit->type <= UNIT_MECH) {
        continue;
      }
      if (!(Ov000_gUnitStats[unit->type].moveGroup &
            (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
        continue;
      }
      if (Ov000_gUnitStats[unit->type].minRange == ATTACK_NONE) {
        continue;
      }
      if (MAP_STATE->rangeMap[unit->y][unit->x] != -1) {
        count++;
      }
    }
  }
  return count;
}

/* Counts the ground units of every player in the game on another team than
 * the current player that stand on a tile the range map reaches. Units being
 * carried are skipped. Only slots 1-50 of each player count. */
int Ai_CountEnemyGroundUnitsInRange(void) {
  int count = 0;
  int player;
  int slot;
  MapUnit* unit;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
        MAP_STATE->players[player].team) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &MAP_STATE->units[slot];
      if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED)) {
        continue;
      }
      if (!(Ov000_gUnitStats[unit->type].moveGroup &
            (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
        continue;
      }
      if (MAP_STATE->rangeMap[unit->y][unit->x] != -1) {
        count++;
      }
    }
  }
  return count;
}

/* TRUE when a ground unit stands on a tile the range map reaches and is
 * either an Infantry or Mech of the current player's team, or an enemy
 * ground unit of any other type. Only slots 1-50 of each player count. */
BOOL Ai_TeamFootOrEnemyVehicleInRange(void) {
  int player;
  int slot;
  MapUnit* unit;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &MAP_STATE->units[slot];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_GROUND)) {
        continue;
      }
      if (MAP_STATE->rangeMap[unit->y][unit->x] == -1) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
          MAP_STATE->players[player].team) {
        if (unit->type <= UNIT_MECH) {
          continue;
        }
      } else {
        if (unit->type > UNIT_MECH) {
          continue;
        }
      }
      return TRUE;
    }
  }
  return FALSE;
}
