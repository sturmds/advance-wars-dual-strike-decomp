#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/range_map.h"
#include "game/map_unit.h"
#include "game/math.h"
#include "game/ai_site.h"
#include "game/ai/ai_site_pick.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai/ai_build_checks.h"
#include "game/ai/ai_count_units.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */
#define UNLIMITED_MOVE 110

#define CRUISER_GUARD_HP 40

#define OWNER(unit) MAP_UNIT_OWNER(MAP_STATE, unit)

#define UNIT_OWNER(slot) (((slot) >> 6) + 1)

/* RangeMap_FillMoves movement; guess: high enough to act as no limit.
 * Tiles next to the reach get ADJACENT. */
#define ANY_DISTANCE 110

#define ADJACENT 111

/* Result bit: the landmass has a port, or a shoal, and the current player
 * has a Lander or the AI profile wants sea transport (for a shoal, only
 * when the player started with a sea factory). */
#define LANDMASS_SEA_TRANSPORT 0x100

#define PLAYER_AT(players, offset) ((Player*)((u32)(players) + (offset)))

/* needed to match: enum constants keep the cmp/moveq/movne order */
enum { SAME_TEAM, ENEMY };

BOOL Player_IsInGame(u8 player);

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

void RangeMap_MarkAdjacentTiles(s8 mark);

BOOL Unit_IsCurrentPlayers(s16 unitSlot);

int Player_GetUnitMoveRange(int player, int unitType);

int UnitType_BestBaseDamage(int unitType, int targetType);

int Unit_GetCoMaxRange(MapUnit* unit);

BOOL Map_IsCapturableByActingUnit(s16 x, s16 y);

BOOL Unit_IsEnemyTile(int unitIndex, s16 x, s16 y);

/* Rates the landmass a Tank could reach from site (x, y). Returns 0 when it
 * holds a pipe seam, an enemy base, an enemy ground unit other than
 * Infantry and Mech, or has an enemy Piperunner next to it. Otherwise
 * returns 1 when a unit on it stands on a tile the acting unit can capture,
 * else the current player's Infantry and Mech on it plus 1. Either value
 * can have LANDMASS_SEA_TRANSPORT set. */
int AiSite_RateLandmass(int x, int y) {
  int capturable;
  int footUnits;
  int wantsTransport;
  int tx;
  int ty;
  int range;
  MapTiles* map;
  int tile;
  int slot;
  u32 type;

  capturable = 0;
  footUnits = 0;
  wantsTransport = 0;
  RangeMap_FillMoves(x, y, UNIT_TANK, ANY_DISTANCE, 0);
  RangeMap_MarkAdjacentTiles(ADJACENT);
  for (ty = 0; ty < MAP_STATE->unk_1c4->mapHeight; ty++) {
    for (tx = 0; tx < MAP_STATE->unk_1c4->mapWidth; tx++) {
      range = MAP_STATE->rangeMap[ty][tx];
      if (range < 0) {
        continue;
      }
      map = MAP_STATE->mapTiles;
      tile = MAP_TILE_INDEX(tx, ty);
      if (MAP_TILE_TERRAIN(map->tiles[tile]) == TERRAIN_PIPE_SEAM) {
        return 0;
      }
      if (range > ANY_DISTANCE) {
        slot = map->unitAt[tile];
        if (slot == 0) {
          continue;
        }
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
            MAP_STATE->players[UNIT_OWNER(slot)].team) {
          continue;
        }
        if (MAP_STATE->units[slot].type != UNIT_PIPERUNNER) {
          continue;
        }
        return 0;
      }
      /* needed to match: the original reads currentUnitBase with ldrsh */
      if (MAP_TILE_TERRAIN(map->tiles[tile]) == TERRAIN_BASE &&
          Unit_IsEnemyTile((s16)MAP_STATE->currentUnitBase, tx, ty) == TRUE) {
        return 0;
      }
      if (!wantsTransport) {
        if (MAP_TILE_TERRAIN(
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(tx, ty)]) ==
            TERRAIN_PORT) {
          if (Ai_CountUnitsOfType(UNIT_LANDER) != 0) {
            wantsTransport = 1;
          } else if (AiProfile_WantsSeaTransport() == TRUE) {
            wantsTransport = 1;
          }
        }
        if (!wantsTransport &&
            MAP_TILE_TERRAIN(
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(tx, ty)]) ==
                TERRAIN_SHOAL) {
          if (Ai_CountUnitsOfType(UNIT_LANDER) != 0) {
            wantsTransport = 1;
          } else if ((gAiTurnState->factoryDomainsOwn & AI_DOMAIN_SEA) &&
                     AiProfile_WantsSeaTransport() == TRUE) {
            wantsTransport = 1;
          }
        }
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(tx, ty)];
      if (slot == 0) {
        continue;
      }
      if (Map_IsCapturableByActingUnit(tx, ty) == TRUE) {
        capturable = 1;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
          MAP_STATE->players[UNIT_OWNER(slot)].team) {
        type = MAP_STATE->units[slot].type;
        if ((Ov000_gUnitStats[type].moveGroup &
             (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) &&
            type > UNIT_MECH) {
          return 0;
        }
      } else if (Unit_IsCurrentPlayers(slot) == TRUE &&
                 MAP_STATE->units[slot].type <= UNIT_MECH) {
        footUnits++;
      }
    }
  }
  if (capturable) {
    return wantsTransport ? LANDMASS_SEA_TRANSPORT | 1 : 1;
  }
  return wantsTransport ? (footUnits + 1) | LANDMASS_SEA_TRANSPORT
                        : footUnits + 1;
}

/* Rates the landmass of the base AiSite_PickForBuild picks (see
 * AiSite_RateLandmass for the result), or 0 when there is none. */
int AiSite_RateBaseLandmass(void) {
  int index = AiSite_PickForBuild(SITE_BASE);

  if (index == -1) {
    return 0;
  }
  return AiSite_RateLandmass(data_0229e5bc[index].x, data_0229e5bc[index].y);
}

/* Rates the danger to a unit of unitType built at (x, y), for the AI's build
 * choice (AiSite_ChooseUnitToBuild and AiTurn_PickUnitToBuild compare it with
 * 90). For an air unit it first adds up the hp of the current team's Fighters
 * that can reach the site (cover). Then each enemy unit 1 to 9 tiles away adds
 * best base damage * hp / 100 when it can hit the site:
 * - an enemy Fighter or Stealth against an air unit, while cover is below
 *   100, when it can move next to the site; against a Fighter or Stealth only
 *   the largest such threat counts;
 * - a Cruiser against air units and Black Boats, Anti-Air against air units
 *   (half for a Bomber or Stealth), a Sub against Battleships and Carriers,
 *   when it can move next to the site;
 * - any unit with a max range above 1 that has the site in range. */
int AiBuild_RateSiteDanger(int x, int y, int unitType) {
  /* needed to match: declaration order sets the stack slots */
  int maxY;
  int damage;
  int maxX;
  int total;
  int minY;
  int ty;
  int col;
  int minX;
  u8 slot;
  int row;
  int airMax;
  u16 savedUnk1e;
  MapUnit* unit;
  u16 cover;
  int tx;
  int dist;
  int range;

  total = 0;
  airMax = 0;
  cover = 0;
  savedUnk1e = MAP_STATE->unk_1e;
  if (Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) {
    RangeMap_FillMoves(
        x, y, UNIT_FIGHTER,
        Player_GetUnitMoveRange(MAP_STATE->currentPlayer, UNIT_FIGHTER), 1);
    RangeMap_MarkAdjacentTiles(111);
    for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
      for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
        if (MAP_STATE->rangeMap[row][col] < 0) {
          continue;
        }
        slot = MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(col, row)];
        if (slot == 0) {
          continue;
        }
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE
                ->players[(MAP_STATE->mapTiles
                               ->unitAt[MAP_TILE_INDEX(col, row)] >>
                           6) +
                          1]
                .team) {
          continue;
        }
        /* needed to match: the slot read again, not `slot`, so that
                 * units is read through the same state pointer as players */
        unit = &MAP_STATE->units[MAP_STATE->mapTiles
                                     ->unk_508[MAP_TILE_INDEX(col, row)]];
        if (unit->type != UNIT_FIGHTER) {
          continue;
        }
        cover += unit->hp;
      }
    }
  }
  minX = x - 9;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - 9;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 9 < maxX) {
    maxX = x + 9;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 9 < maxY) {
    maxY = y + 9;
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
      if (dist < 1 || dist > 9) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(tx, ty)];
      if (slot == 0) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE
              ->players[(MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(tx, ty)] >>
                         6) +
                        1]
              .team) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      if (cover < 100 &&
          (unit->type == UNIT_FIGHTER || unit->type == UNIT_STEALTH) &&
          (Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR)) {
        MAP_STATE->unk_1e = OWNER(unit);
        RangeMap_FillMoves(unit->x, unit->y, unit->type,
                           Player_GetUnitMoveRange(OWNER(unit), unit->type), 1);
        RangeMap_MarkAdjacentTiles(111);
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        damage = HwDiv32(
            UnitType_BestBaseDamage(unit->type, unitType) * unit->hp, 100);
        if (unitType != UNIT_FIGHTER && unitType != UNIT_STEALTH) {
          total += damage;
        } else if (damage > airMax) {
          airMax = damage;
        }
        continue;
      }
      if (((Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) ||
           unitType == UNIT_BLACK_BOAT) &&
              unit->type == UNIT_CRUISER ||
          (Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) &&
              unit->type == UNIT_ANTI_AIR ||
          (unitType == UNIT_BATTLESHIP || unitType == UNIT_CARRIER) &&
              unit->type == UNIT_SUB) {
        MAP_STATE->unk_1e = OWNER(unit);
        RangeMap_FillMoves(unit->x, unit->y, unit->type,
                           Player_GetUnitMoveRange(OWNER(unit), unit->type), 1);
        RangeMap_MarkAdjacentTiles(111);
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        damage = HwDiv32(
            UnitType_BestBaseDamage(unit->type, unitType) * unit->hp, 100);
        if ((unitType == UNIT_BOMBER || unitType == UNIT_STEALTH) &&
            unit->type == UNIT_ANTI_AIR) {
          damage >>= 1;
        }
        total += damage;
        continue;
      }
      range = Unit_GetCoMaxRange(unit);
      if (range <= 1 || range < dist ||
          Ov000_gUnitStats[unit->type].minRange > dist) {
        continue;
      }
      total += HwDiv32(UnitType_BestBaseDamage(unit->type, unitType) * unit->hp,
                       100);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
  return total + airMax;
}

/* For each Sub of the player: fills rangeMap with the tiles it can reach or
 * stand next to, then adds 1 to MapTiles.rangeScratch on each of those
 * tiles. A Sub with a Cruiser of the current player's team in that area is
 * skipped. Ai_CountEnemySubReach calls it for each enemy player. */
void Ai_AddSubReach(u8 player) {
  /* needed to match: the second scan has its own x and y */
  int slot;
  MapUnit* unit;
  MapUnit* other;
  int x;
  int y;
  int markX;
  int markY;

  MAP_STATE->unk_1e = player;
  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    unit = &MAP_STATE->units[slot];
    /* needed to match: the UNIT_NONE test, though the Sub test covers it */
    if (unit->type == UNIT_NONE || unit->type != UNIT_SUB) {
      continue;
    }
    RangeMap_FillMoves(unit->x, unit->y, unit->type,
                       Player_GetUnitMoveRange(OWNER(unit), unit->type), 1);
    RangeMap_MarkAdjacentTiles(111);
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        other = &MAP_STATE
                     ->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
                MAP_STATE->players[OWNER(other)].team &&
            other->type == UNIT_CRUISER) {
          goto nextSub;
        }
      }
    }
    for (markY = 0; markY < MAP_STATE->unk_1c4->mapHeight; markY++) {
      for (markX = 0; markX < MAP_STATE->unk_1c4->mapWidth; markX++) {
        if (MAP_STATE->rangeMap[markY][markX] >= 0) {
          MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(markX, markY)]++;
        }
      }
    }
  nextSub:;
  }
}

/* Clears MapTiles.rangeScratch, then lets Ai_AddSubReach add 1 there for each
 * enemy Sub that can reach or get next to the tile. A Sub with a Cruiser of
 * the current player's team inside that reach is skipped. */
void Ai_CountEnemySubReach(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = MAP_STATE->unk_1e;
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_Fill(0);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      Ai_AddSubReach(player);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
}

/* For each Sub of `player` with more than 20 hp: fills the range map with its
 * moves over the whole map plus the tiles next to them. If one of those tiles
 * holds a current player's Cruiser with CRUISER_GUARD_HP or more, the Sub is
 * skipped; otherwise each reached tile of MapTiles.rangeScratch keeps the lower
 * of its value and the Sub's move cost. */
void Ai_MarkEnemySubReach(u8 player) {
  MapUnit* unit;
  MapUnit* other;
  int slot;
  int fillX; /* needed to match: the second scan has its own column counter */
  int x;
  int y;

  MAP_STATE->unk_1e = player;
  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type == UNIT_NONE || unit->type != UNIT_SUB || unit->hp <= 20) {
      continue;
    }
    RangeMap_FillMoves(unit->x, unit->y, unit->type, UNLIMITED_MOVE, 0);
    RangeMap_MarkAdjacentTiles(111);
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        if (Unit_IsCurrentPlayers(
                MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(x, y)]) == 0) {
          continue;
        }
        other = &MAP_STATE
                     ->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
        if (other->type == UNIT_CRUISER && other->hp >= CRUISER_GUARD_HP) {
          goto next;
        }
      }
    }
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (fillX = 0; fillX < MAP_STATE->unk_1c4->mapWidth; fillX++) {
        if (MAP_STATE->rangeMap[y][fillX] < 0) {
          continue;
        }
        if (MAP_STATE->rangeMap[y][fillX] <=
            MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(fillX, y)]) {
          MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(fillX, y)] =
              MAP_STATE->rangeMap[y][fillX];
        }
      }
    }
  next:;
  }
}

/* Fills MapTiles.rangeScratch with 127, then marks the reach of every enemy
 * player's Subs on it (Ai_MarkEnemySubReach). */
void Ai_BuildEnemySubReachMap(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = MAP_STATE->unk_1e;
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_Fill(127);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      Ai_MarkEnemySubReach(player);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
}

/* 1 when an Oozium of another team than the current player's stands on the tile. */
int AiTile_HasEnemyOozium(int x, int y) {
  int slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];

  if (MAP_STATE->units[slot].type != UNIT_OOZIUM) {
    return 0;
  }
  return MAP_STATE->players[MAP_STATE->currentPlayer].team ==
                 MAP_STATE->players[(slot >> 6) + 1].team
             ? SAME_TEAM
             : ENEMY;
}

/* 1 when a unit stands on the tile and belongs to the current player's team. */
int AiTile_HasOwnTeamUnit(int x, int y) {
  int slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];

  if (slot == 0) {
    return 0;
  }
  return MAP_STATE->players[MAP_STATE->currentPlayer].team ==
         MAP_STATE->players[(slot >> 6) + 1].team;
}

/* 1 when a unit of the current player's team stands on a tile next to (x, y). */
int AiTile_HasAdjacentOwnTeamUnit(int x, int y) {
  if (AiTile_HasOwnTeamUnit(x - 1, y) == 1) {
    return 1;
  }
  if (AiTile_HasOwnTeamUnit(x + 1, y) == 1) {
    return 1;
  }
  if (AiTile_HasOwnTeamUnit(x, y - 1) == 1) {
    return 1;
  }
  return AiTile_HasOwnTeamUnit(x, y + 1) == 1;
}

/* 1 when an enemy Oozium stands next to (x, y) and no unit of the current
 * player's team stands next to that Oozium. Always 0 while the current
 * player's tag power runs its front half. The AiSite_Pick* functions skip
 * sites where this is 1. */
int AiSite_NearUnguardedOozium(int x, int y) {
  if (MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf ==
      TAG_POWER_FRONT) {
    return 0;
  }
  if (AiTile_HasEnemyOozium(x - 1, y) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x - 1, y) == 0) {
    return 1;
  }
  if (AiTile_HasEnemyOozium(x + 1, y) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x + 1, y) == 0) {
    return 1;
  }
  if (AiTile_HasEnemyOozium(x, y - 1) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x, y - 1) == 0) {
    return 1;
  }
  if (AiTile_HasEnemyOozium(x, y + 1) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x, y + 1) == 0) {
    return 1;
  }
  return 0;
}
