#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/math.h"
#include "game/ai_unit_action.h"
#include "game/range_map.h"
#include "game/map_object.h"
#include "game/tile_point.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/ai/ai_threat.h"
#include "game/ai/ai_best_damage.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define ACTING (MAP_STATE->actingUnit)
#define TILE_INDEX(x, y) (MAP_STATE->mapTiles->rowStart[y] + (x))
#define OWNER(unit) MAP_UNIT_OWNER(MAP_STATE, unit)
#define MAX_RANGE(unit) GetUnitBaseMaxRange(OWNER(unit), (unit)->type)
/* max range with no power on */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT (MAP_STATE->players[MAP_STATE->currentPlayer])
#define FOGGY \
  (MAP_STATE->unk_1c4->fog != 0 || MAP_STATE->unk_1c4->weather == WEATHER_RAIN)

/* Per object variant (bits 14-15 of the object's second halfword): its
 * tiles, ended by an x of TILE_POINT_END. */
extern TilePointU16* data_02167e98[];

int UnitType_GetAttackKind(u8 unitType);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
int Player_GetRepairHp(int playerIndex, int slot, int powerState);
int GetUnitBaseMaxRange(int player, int unitType);
BOOL Player_IsInGame(u8 player);
BOOL CounterFirst(int player);
int UnitType_BestBaseDamage(int unitType, int targetType);
int Player_GetUnitMoveRange(int player, int unitType);
/* movement is s16 in fill_unit_moves.c; int here, needed to match (no sign extension) */
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);
/* fog, weather and terrain check of a tile */
BOOL UnitType_CanEnterTerrain(int unitType, u8 terrain);
int abs(int x);
int Unit_GetUsableMovement(MapUnit* unit);
BOOL CanUseSuperPower(int player, int slot);

/* Number of tiles next to (x, y) that the acting unit can reach: the range
 * map holds a move cost there that is not above the unit's usable movement
 * (gAiTurnState->unk_05). */
int Ai_CountReachableAdjacentTiles(int x, int y) {
  /* needed to match: declaration order sets registers and stack slots */
  int maxX;
  int maxY;
  int cy;
  int count;
  int minX;
  int cx;
  int distance;
  int cost;

  count = 0;
  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  cy = y - 1;
  if (cy < 0) {
    cy = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
  }
  for (; cy <= maxY; cy++) {
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
      if (distance < 1 || distance > 1) {
        continue;
      }
      cost = MAP_STATE->rangeMap[cy][cx];
      if (cost < 0) {
        continue;
      }
      if (cost <= gAiTurnState->unk_05) {
        count++;
      }
    }
  }
  return count;
}

/* Whether the AI player lets a unit of this type take the threat maps into
 * account (the callers skip the threat code when this is FALSE). Never when the
 * front CO's half of a tag power is on or the AI behaviour is STRIKE. Also not
 * for an Eagle player's units other than Infantry and Mech, while the Super CO
 * Power of each CO it has is ready, early in the turn (before AI phase 32 with
 * fog or rain, 27 without). Meaning of the phase numbers unknown. */
BOOL Ai_UnitUsesThreatMaps(int unitType) {
  u32 hasPartner; /* needed to match: a u32 gets a separate compare */

  if (CURRENT.tagPowerHalf == TAG_POWER_FRONT) {
    return FALSE;
  }
  if (CURRENT.aiBehaviour == AI_BEHAVIOUR_STRIKE) {
    return FALSE;
  }
  if ((CURRENT.front.id & PLAYER_CO_ID_MASK) == CO_EAGLE &&
      unitType != UNIT_INFANTRY && unitType != UNIT_MECH) {
    if ((FOGGY && gAiTurnState->phase < 32) ||
        (!FOGGY && gAiTurnState->phase < 27)) {
      hasPartner = CURRENT.back.id != 0;
      if (!hasPartner) {
        if (CanUseSuperPower(MAP_STATE->currentPlayer, 0)) {
          return FALSE;
        }
      } else if (CanUseSuperPower(MAP_STATE->currentPlayer, 0) &&
                 CanUseSuperPower(MAP_STATE->currentPlayer, 1)) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

/* Best base damage, raised 1.5 times for a Fighter against a Fighter and 2 times
 * for an Anti-Air against a Fighter, so the AI rates these attacks higher. */
int Ai_RatedBaseDamage(int unitType, int targetType) {
  int damage = UnitType_BestBaseDamage(unitType, targetType);

  if (targetType == UNIT_FIGHTER && unitType == UNIT_FIGHTER) {
    damage = damage * 3 >> 1;
  }
  if (targetType == UNIT_FIGHTER && unitType == UNIT_ANTI_AIR) {
    damage = damage * 2;
  }
  return damage;
}

/* TRUE when the enemy unit is close enough to matter to the acting unit: the
 * tile distance is at most the acting unit's movement plus the enemy's move
 * range plus 1 (a direct attacker), or plus its max range (anything else), plus
 * slack. The unit in actingUnit adds 1 to slack when it carries cargo and cannot attack. */
BOOL Ai_EnemyWithinReach(MapUnit* acting, MapUnit* enemy, int slack) {
  int distance;
  int reach;

  if (MAP_STATE->actingUnit->cargo[0] != 0) {
    if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) ==
        ATTACK_KIND_NONE) {
      slack++;
    }
  }
  distance = abs(acting->x - enemy->x) + abs(acting->y - enemy->y);
  if (UnitType_GetAttackKind(enemy->type) == ATTACK_KIND_DIRECT) {
    reach = slack + (Unit_GetUsableMovement(acting) +
                     Player_GetUnitMoveRange(MAP_UNIT_OWNER(MAP_STATE, enemy),
                                             enemy->type) +
                     1);
  } else {
    reach = slack + (Unit_GetUsableMovement(acting) +
                     GetUnitBaseMaxRange(MAP_UNIT_OWNER(MAP_STATE, enemy),
                                         enemy->type));
  }
  if (distance > reach) {
    return FALSE;
  }
  return TRUE;
}

/* For an Oozium the current player can see (Player_CanSeeUnitAt), sets threatSum to
 * 255 on each of the four tiles next to it that UnitType_CanEnterTerrain accepts for an
 * Oozium. */
void Ai_AddOoziumThreat(MapUnit* unit) {
  int maxX;
  int y;
  int maxY;
  int x;
  int minX;
  int distance;

  if (unit->type != UNIT_OOZIUM) {
    return;
  }
  if (!Player_CanSeeUnitAt(MAP_STATE->currentPlayer, unit->x, unit->y)) {
    return;
  }
  /* Needed to match: minX and the first row clamp in different forms, and
     * the row loop starts from y itself (no minY), which sets the register
     * and stack slot choice. */
  minX = unit->x - 1;
  if (minX < 0) {
    minX = 0;
  }
  if (unit->y - 1 < 0) {
    y = 0;
  } else {
    y = unit->y - 1;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (unit->x + 1 < maxX) {
    maxX = unit->x + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (unit->y + 1 < maxY) {
    maxY = unit->y + 1;
  }
  for (; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      distance = 0;
      if (unit->x - x > 0) {
        distance += unit->x - x;
      } else {
        distance += x - unit->x;
      }
      if (unit->y - y > 0) {
        distance += unit->y - y;
      } else {
        distance += y - unit->y;
      }
      if (distance >= 1 && distance <= 1) {
        if (UnitType_CanEnterTerrain(
                UNIT_OOZIUM,
                MAP_TILE_TERRAIN(
                    MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]))) {
          MAP_STATE->mapTiles->threatSum[MAP_TILE_INDEX(x, y)] = 255;
        }
      }
    }
  }
}

/* Sets threatSum to 255 on the tiles listed for the first active object of
 * kind 2 (unk_06 is 1), for unit types Ai_UnitUsesThreatMaps accepts. */
void Ai_MarkObjectTileThreat(void) {
  MapObject* object;
  TilePoint* tile; /* needed to match: the code reads the tiles as s16 */

  if (!Ai_UnitUsesThreatMaps(MAP_STATE->actingUnit->type)) {
    return;
  }
  for (object = MAP_STATE->objects; object->kind != 0; object++) {
    if (object->kind == 2 && object->state == 1) {
      tile = (TilePoint*)data_02167e98[object->variant];
      for (;; tile++) {
        if ((u16)tile->x == TILE_POINT_END) {
          return;
        }
        MAP_STATE->mapTiles
            ->threatSum[tile->x + MAP_STATE->mapTiles->rowStart[tile->y]] = 255;
      }
    }
  }
}

/* With objects attackable (AiTargetFlags) and a unit type that cares
 * (Ai_UnitUsesThreatMaps), points the range map at threatSum and hands each active
 * object (state 1) to MapObject_MarkFireRange with -1. */
void Ai_AddObjectThreat(void) {
  MapObject* object;

  if (!AI_TARGET_FLAGS->attackObjects) {
    return;
  }
  if (!Ai_UnitUsesThreatMaps(MAP_STATE->actingUnit->type)) {
    return;
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->threatSum);
  for (object = MAP_STATE->objects; object->kind != MAP_OBJECT_NONE; object++) {
    if (object->state == 1) {
      MapObject_MarkFireRange(object, -1);
    }
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
}

/* Adds the units of one enemy player to the threat maps of the acting AI
 * unit. Each enemy unit that can hurt the acting unit gets a threat of
 * damage * hp / 100, capped at 255; hp counts 20 more on Unit_IsBlackHoleOnFlaggedTile
 * tiles and the repair it would get, capped at 100. A direct attacker raises
 * MapTiles.threatMax on the tiles it can reach or stand next to; an indirect
 * attacker adds its threat to MapTiles.threatSum on the tiles in its range. */
void Ai_AddEnemyThreat(u8 player) {
  MapUnit* unit;
  int damage;
  int threat;
  int x;
  int y;
  int targetX;
  int dist;
  int targetY;
  int minX;
  int minY;
  int maxX;
  int maxY;
  BOOL skipDirectEnemies = FALSE;
  BOOL counterFirst = skipDirectEnemies;
  int slot;

  if (!Ai_UnitUsesThreatMaps(ACTING->type)) {
    return;
  }
  if (UnitType_GetAttackKind(ACTING->type) == ATTACK_KIND_DIRECT) {
    counterFirst = CounterFirst(
        (u8)MAP_STATE->currentPlayer); /* needed to match: the cast */
  }
  /* Assault AI: direct and unarmed units ignore enemy direct attackers */
  if (MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour ==
      AI_BEHAVIOUR_ASSAULT) {
    if (UnitType_GetAttackKind(ACTING->type) == ATTACK_KIND_DIRECT ||
        UnitType_GetAttackKind(ACTING->type) == ATTACK_KIND_NONE) {
      skipDirectEnemies = TRUE;
    }
  }
  MAP_STATE->unk_1e = player;
  /* 50 unit slots per player */
  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type == UNIT_NONE) {
      continue;
    }
    Ai_AddOoziumThreat(unit);
    if (unit->flags & (MAP_UNIT_CARRIED | MAP_UNIT_PARALYSED)) {
      continue;
    }
    damage = Ai_RatedBaseDamage(unit->type, ACTING->type);
    if (damage < 25) {
      continue;
    }
    if (!Ai_EnemyWithinReach(ACTING, unit, 0)) {
      continue;
    }
    if (ACTING->flags & MAP_UNIT_DIVED) {
      if (!Ai_BestDamageVsDived(unit->type, ACTING->type, 0)) {
        continue;
      }
    }
    if (unit->type == UNIT_SUB || unit->type == UNIT_STEALTH) {
      if (!Unit_IsVisibleToCurrentPlayer(slot)) {
        continue;
      }
    }
    if (!Player_CanSeeUnitAt(MAP_STATE->currentPlayer, unit->x, unit->y)) {
      continue;
    }
    /* needed to match: threat holds the hp until the division */
    threat = unit->hp;
    if (Unit_IsBlackHoleOnFlaggedTile(unit, unit->x, unit->y) == TRUE) {
      threat += 20;
    }
    if (Unit_CanRepairAt(unit, unit->x, unit->y) == TRUE) {
      threat += Player_GetRepairHp(OWNER(unit), 0, 0) * 10;
    }
    if (threat > 100) {
      threat = 100;
    }
    threat = HwDiv32(damage * threat, 100);
    if (threat > 255) {
      threat = 255;
    }
    if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
      if (skipDirectEnemies == TRUE) {
        continue;
      }
      /* the acting unit strikes first: skip enemies its base damage, scaled by its hp, takes to 45% or more */
      if (counterFirst == TRUE) {
        if (UnitType_BestBaseDamage(ACTING->type, unit->type) * ACTING->hp >=
            4500) {
          continue;
        }
      }
      RangeMap_FillMoves(unit->x, unit->y, unit->type,
                         Player_GetUnitMoveRange(OWNER(unit), unit->type), 1);
      RangeMap_MarkAdjacentTiles(111);
      for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
          if (MAP_STATE->rangeMap[y][x] < 0) {
            continue;
          }
          /* tiles that fail Player_CanSeeUnitAt count only against air units */
          if (Player_CanSeeUnitAt(MAP_STATE->unk_1e, x, y) ||
              (Ov000_gUnitStats[ACTING->type].moveGroup & MOVE_GROUP_AIR)) {
            if (threat > MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)]) {
              MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)] = threat;
            }
          }
        }
      }
    } else {
      /* ground units ignore indirect attackers they cannot see */
      if (MAP_STATE->mapTiles->inSight[TILE_INDEX(unit->x, unit->y)] == 0 &&
          (Ov000_gUnitStats[ACTING->type].moveGroup &
           (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
        continue;
      }
      if (unit->x - MAX_RANGE(unit) < 0) {
        minX = 0;
      } else {
        minX = unit->x - MAX_RANGE(unit);
      }
      if (unit->y - MAX_RANGE(unit) < 0) {
        minY = 0;
      } else {
        minY = unit->y - MAX_RANGE(unit);
      }
      maxX = unit->x + MAX_RANGE(unit) < MAP_STATE->unk_1c4->mapWidth - 1
                 ? unit->x + MAX_RANGE(unit)
                 : MAP_STATE->unk_1c4->mapWidth - 1;
      maxY = unit->y + MAX_RANGE(unit) < MAP_STATE->unk_1c4->mapHeight - 1
                 ? unit->y + MAX_RANGE(unit)
                 : MAP_STATE->unk_1c4->mapHeight - 1;
      for (targetY = minY; targetY <= maxY; targetY++) {
        for (targetX = minX; targetX <= maxX; targetX++) {
          dist = 0;
          if (unit->x - targetX > 0) {
            dist += unit->x - targetX;
          } else {
            dist += targetX - unit->x;
          }
          if (unit->y - targetY > 0) {
            dist += unit->y - targetY;
          } else {
            dist += targetY - unit->y;
          }
          if (dist < Ov000_gUnitStats[unit->type].minRange ||
              dist > MAX_RANGE(unit)) {
            continue;
          }
          if (Player_CanSeeUnitAt(MAP_STATE->unk_1e, targetX, targetY) ||
              (Ov000_gUnitStats[ACTING->type].moveGroup & MOVE_GROUP_AIR)) {
            /* needed to match: the sum is kept in damage */
            damage =
                MAP_STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] +
                threat;
            if (damage > 255) {
              damage = 255;
            }
            MAP_STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] =
                damage;
          }
        }
      }
    }
  }
}

/* Sets MapTiles.threatMax and threatSum to 0 for every tile. */
void Ai_ClearThreatMaps(void) {
  TileMap_Fill(MAP_STATE->mapTiles->threatMax, 0);
  TileMap_Fill(MAP_STATE->mapTiles->threatSum, 0);
}

/* Fills the per-tile maps the AI checks before moving (MapTiles.threatMax
 * and threatSum) from every enemy player still in the game. Runs once per
 * queued unit: AiTurn_RunQueue clears threatMapsBuilt. */
void Ai_BuildThreatMaps(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = MAP_STATE->unk_1e;
  if (AI_TARGET_FLAGS->threatMapsBuilt) {
    return;
  }
  AI_TARGET_FLAGS->threatMapsBuilt = TRUE;
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      Ai_AddEnemyThreat(player);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
}

/* A variant of Ai_AddEnemyThreat that only adds indirect attackers: each
 * non-air enemy unit of the player that is not a direct attacker adds its
 * threat (damage * hp / 100, capped at 255) to MapTiles.threatSum on the
 * tiles in its range. Differences: the Ai_UnitUsesThreatMaps check uses a
 * Fighter, not the acting unit's type; no assault, counter-first, dived,
 * Sub/Stealth or in-sight checks; Ai_EnemyWithinReach gets 1. */
void Ai_AddIndirectEnemyThreat(u8 player) {
  MapUnit* unit;
  int damage;
  int threat;
  int targetX;
  int dist;
  int targetY;
  int minX;
  int minY;
  int maxX;
  int maxY;
  int slot;

  if (!Ai_UnitUsesThreatMaps(UNIT_FIGHTER)) {
    return;
  }
  MAP_STATE->unk_1e = player;
  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type == UNIT_NONE) {
      continue;
    }
    Ai_AddOoziumThreat(unit);
    if (unit->flags & (MAP_UNIT_CARRIED | MAP_UNIT_PARALYSED)) {
      continue;
    }
    if (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR) {
      continue;
    }
    damage = Ai_RatedBaseDamage(unit->type, ACTING->type);
    if (damage < 25) {
      continue;
    }
    if (!Ai_EnemyWithinReach(ACTING, unit, 1)) {
      continue;
    }
    if (!Player_CanSeeUnitAt(MAP_STATE->currentPlayer, unit->x, unit->y)) {
      continue;
    }
    threat = unit->hp;
    if (Unit_IsBlackHoleOnFlaggedTile(unit, unit->x, unit->y) == TRUE) {
      threat += 20;
    }
    if (Unit_CanRepairAt(unit, unit->x, unit->y) == TRUE) {
      threat += Player_GetRepairHp(OWNER(unit), 0, 0) * 10;
    }
    if (threat > 100) {
      threat = 100;
    }
    threat = HwDiv32(damage * threat, 100);
    if (threat > 255) {
      threat = 255;
    }
    if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
      continue;
    }
    if (unit->x - MAX_RANGE(unit) < 0) {
      minX = 0;
    } else {
      minX = unit->x - MAX_RANGE(unit);
    }
    if (unit->y - MAX_RANGE(unit) < 0) {
      minY = 0;
    } else {
      minY = unit->y - MAX_RANGE(unit);
    }
    maxX = unit->x + MAX_RANGE(unit) < MAP_STATE->unk_1c4->mapWidth - 1
               ? unit->x + MAX_RANGE(unit)
               : MAP_STATE->unk_1c4->mapWidth - 1;
    maxY = unit->y + MAX_RANGE(unit) < MAP_STATE->unk_1c4->mapHeight - 1
               ? unit->y + MAX_RANGE(unit)
               : MAP_STATE->unk_1c4->mapHeight - 1;
    for (targetY = minY; targetY <= maxY; targetY++) {
      for (targetX = minX; targetX <= maxX; targetX++) {
        dist = 0;
        if (unit->x - targetX > 0) {
          dist += unit->x - targetX;
        } else {
          dist += targetX - unit->x;
        }
        if (unit->y - targetY > 0) {
          dist += unit->y - targetY;
        } else {
          dist += targetY - unit->y;
        }
        if (dist < Ov000_gUnitStats[unit->type].minRange ||
            dist > MAX_RANGE(unit)) {
          continue;
        }
        /* tiles that fail Player_CanSeeUnitAt count only against air units */
        if (Player_CanSeeUnitAt(MAP_STATE->unk_1e, targetX, targetY) ||
            (Ov000_gUnitStats[ACTING->type].moveGroup & MOVE_GROUP_AIR)) {
          damage =
              MAP_STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] +
              threat;
          if (damage > 255) {
            damage = 255;
          }
          MAP_STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] = damage;
        }
      }
    }
  }
}
