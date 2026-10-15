#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/math.h"
#include "game/range_map.h"
#include "game/ai/ai_threat_maps.h"
#include "game/ai/ai_threat.h"
#include "game/ai/ai_best_damage.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define TILE_INDEX(x, y) (MAP_STATE->mapTiles->rowStart[y] + (x))
#define OWNER(unit) MAP_UNIT_OWNER(MAP_STATE, unit)
#define MAX_RANGE(unit) GetUnitBaseMaxRange(OWNER(unit), (unit)->type)
/* an Oozium in sight sets threatSum to 255 on the tiles next to it */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define RANGE(unit) GetUnitBaseMaxRange(MAP_STATE->currentPlayer, (unit)->type)
#define ACTING (MAP_STATE->actingUnit)

BOOL Player_IsInGame(u8 player);
BOOL CounterFirst(int player);
int UnitType_GetAttackKind(u8 unitType);
int UnitType_BestBaseDamage(int unitType, int targetType);
int Player_GetUnitMoveRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
int Player_GetRepairHp(int playerIndex, int slot, int powerState);
int GetUnitBaseMaxRange(int player, int unitType);
void RangeMap_FillUnitMoves(MapUnit* unit);

/* A variant of Ai_AddEnemyThreat for the Black Bomb plans (func_0209b2b0):
 * adds the units of one enemy player to MapTiles.threatMax (direct
 * attackers, on the tiles they can reach or stand next to) and threatSum
 * (indirect attackers, on the tiles in range), with the same threat value.
 * Differences: it runs when Ai_UnitUsesThreatMaps passes for a Black Bomb, skips the
 * counter-first test and the per-tile Player_CanSeeUnitAt and in-sight checks, and
 * marks every tile a non-air enemy threatens with 1 in
 * MapTiles.rangeScratch. skipEnemyCheck nonzero skips Ai_EnemyWithinReach. */
void AiBlackBomb_AddEnemyThreat(u8 player, int skipEnemyCheck) {
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
  int slot;

  if (!Ai_UnitUsesThreatMaps(UNIT_BLACK_BOMB)) {
    return;
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
    if (skipEnemyCheck == 0) {
      if (!Ai_EnemyWithinReach(ACTING, unit, 0)) {
        continue;
      }
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
      RangeMap_FillMoves(unit->x, unit->y, unit->type,
                         Player_GetUnitMoveRange(OWNER(unit), unit->type), 1);
      RangeMap_MarkAdjacentTiles(111);
      for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
          if (MAP_STATE->rangeMap[y][x] < 0) {
            continue;
          }
          if (threat > MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)]) {
            MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)] = threat;
          }
          if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR)) {
            MAP_STATE->mapTiles->rangeScratch[TILE_INDEX(x, y)] = 1;
          }
        }
      }
    } else {
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
          damage =
              MAP_STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] +
              threat;
          if (damage > 255) {
            damage = 255;
          }
          MAP_STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] = damage;
          if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR)) {
            MAP_STATE->mapTiles->rangeScratch[TILE_INDEX(targetX, targetY)] = 1;
          }
        }
      }
    }
  }
}

/* Clears the threat maps (threatMax, threatSum) on tiles that the current
 * player's anti-air units cover, where rangeScratch is 0. A unit counts when
 * it can damage a Fighter, is not carried, has more than 60 hp and has ammo.
 * A direct attacker covers the tiles it can move to; an indirect one the
 * tiles in its attack range. Quirks kept from the original: the minimum
 * range comes from the acting unit's type, not the unit's, and the search
 * box reaches one tile past RANGE(unit) - 1 on each side. */
void Ai_ClearThreatInAntiAirCover(void) {
  int x;
  int minY;
  MapUnit* unit;
  int maxY;
  int slot;
  int tx;
  int dist;
  int y;
  int ty;
  int minX;
  int maxX;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type == 0 ||
        UnitType_BestBaseDamage(unit->type, UNIT_FIGHTER) == 0) {
      continue;
    }
    if ((unit->flags & MAP_UNIT_CARRIED) || unit->hp < 61 || unit->ammo == 0) {
      continue;
    }

    if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
      RangeMap_FillUnitMoves(unit);
      for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
          if (MAP_STATE->rangeMap[y][x] >= 0 &&
              MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)] == 0) {
            MAP_TILES->threatMax[MAP_TILE_INDEX(x, y)] = 0;
            MAP_TILES->threatSum[MAP_TILE_INDEX(x, y)] = 0;
          }
        }
      }
    } else {
      minX = unit->x - RANGE(unit) - 1 < 0 ? 0 : unit->x - RANGE(unit) - 1;
      minY = unit->y - RANGE(unit) - 1 < 0 ? 0 : unit->y - RANGE(unit) - 1;
      maxX = unit->x + RANGE(unit) - 1 < MAP_STATE->unk_1c4->mapWidth - 1
                 ? unit->x + RANGE(unit) - 1
                 : MAP_STATE->unk_1c4->mapWidth - 1;
      maxY = unit->y + RANGE(unit) - 1 < MAP_STATE->unk_1c4->mapHeight - 1
                 ? unit->y + RANGE(unit) - 1
                 : MAP_STATE->unk_1c4->mapHeight - 1;
      for (ty = minY; ty <= maxY; ty++) {
        for (tx = minX; tx <= maxX; tx++) {
          dist = 0;
          if (unit->x - tx > 0) {
            dist += unit->x - tx;
          } else {
            dist += tx - unit->x;
          }
          if (unit->y - ty > 0) {
            dist += unit->y - ty;
          } else {
            dist += ty - unit->y;
          }
          if (dist <
                  Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange + 1 ||
              dist > RANGE(unit) - 1) {
            continue;
          }
          if (MAP_TILES->rangeScratch[MAP_TILE_INDEX(tx, ty)] == 0) {
            MAP_TILES->threatMax[MAP_TILE_INDEX(tx, ty)] = 0;
            MAP_TILES->threatSum[MAP_TILE_INDEX(tx, ty)] = 0;
          }
        }
      }
    }
  }
}

/* Builds the AI threat maps like Ai_BuildThreatMaps for every enemy player
 * still in the game (AiBlackBomb_AddEnemyThreat per player, arg passed through), with
 * the range map switched to rangeScratch filled with 0 meanwhile and to rangeMapTiles
 * after. Unless the current player's AI behaviour is Defense it then calls
 * Ai_ClearThreatInAntiAirCover, and it leaves rangeScratch filled with -1. */
void func_0209b2b0(int arg) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = MAP_STATE->unk_1e;
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_Fill(0);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      AiBlackBomb_AddEnemyThreat(player, arg);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
  if (MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour !=
      AI_BEHAVIOUR_DEFENSE) {
    Ai_ClearThreatInAntiAirCover();
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_Fill(-1);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
}

/* Clears the threat maps and adds the threat of each unit of every enemy
 * player still in the game. Unlike Ai_BuildThreatMaps it does not call
 * Ai_AddObjectThreat and Ai_MarkObjectTileThreat. Units use slots 1 to 50 of the player's
 * block. */
void AiOozium_BuildThreatMaps(void) {
  int player;
  int slot;
  MapUnit* unit;

  Ai_ClearThreatMaps();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
        unit = &MAP_STATE->units[slot];
        if (unit->type != 0) {
          Ai_AddOoziumThreat(unit);
        }
      }
    }
  }
}

/* A variant of Ai_AddEnemyThreat for the Fighter build check
 * (AiTurn_TryBuildFighter, through Ai_BuildFighterThreatMaps): adds the units of one
 * enemy player to the threat maps as seen by a Fighter, whatever the acting
 * unit is. Enemies a Fighter hits for a base damage of 50 or more are
 * skipped. Threat is worked out as in Ai_AddEnemyThreat, but a direct
 * attacker adds it to MapTiles.threatMax (capped at 255) instead of keeping
 * the highest, and there are no Assault, dived, reach or per-tile checks. */
void Ai_AddEnemyThreatToFighter(u8 player) {
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
  BOOL counterFirst;
  int slot;

  if (!Ai_UnitUsesThreatMaps(UNIT_FIGHTER)) {
    return;
  }
  counterFirst = CounterFirst((u8)MAP_STATE->currentPlayer);
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
    damage = Ai_RatedBaseDamage(unit->type, UNIT_FIGHTER);
    if (damage < 25) {
      continue;
    }
    if (unit->type == UNIT_SUB || unit->type == UNIT_STEALTH) {
      if (!Unit_IsVisibleToCurrentPlayer(slot)) {
        continue;
      }
    }
    if (!Player_CanSeeUnitAt(MAP_STATE->currentPlayer, unit->x, unit->y)) {
      continue;
    }
    /* enemies a Fighter hits hard are not a threat to it */
    if (UnitType_BestBaseDamage(UNIT_FIGHTER, unit->type) >= 50) {
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
      /* the Fighter strikes first: skip enemies it hits for 45% or more */
      if (counterFirst == TRUE) {
        if (UnitType_BestBaseDamage(UNIT_FIGHTER, unit->type) >= 45) {
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
          damage = MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)] + threat;
          if (damage > 255) {
            damage = 255;
          }
          MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)] = damage;
        }
      }
    } else {
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

void Ai_BuildFighterThreatMaps(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = MAP_STATE->unk_1e;
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      Ai_AddEnemyThreatToFighter(player);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
}

/* End-of-turn variant of Ai_AddEnemyThreat: there is no acting unit, so each
 * enemy unit is rated against an Anti-Air (it runs only when
 * Ai_UnitUsesThreatMaps(UNIT_FIGHTER) passes). Same threat value and the same
 * threatMax / threatSum updates as AiBlackBomb_AddEnemyThreat, including the
 * rangeScratch mark for non-air direct attackers. Direct attackers an Anti-Air
 * deals 50 base damage or more to are skipped, and 45 or more when the current
 * player strikes first (CounterFirst). No per-tile Player_CanSeeUnitAt or in-sight
 * checks. */
void AiEndActions_AddEnemyThreat(u8 player) {
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
  BOOL counterFirst;
  int slot;

  if (!Ai_UnitUsesThreatMaps(UNIT_FIGHTER)) {
    return;
  }
  counterFirst = CounterFirst(
      (u8)MAP_STATE->currentPlayer); /* needed to match: the cast */
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
    damage = Ai_RatedBaseDamage(unit->type, UNIT_ANTI_AIR);
    if (damage < 25) {
      continue;
    }
    if (unit->type == UNIT_SUB || unit->type == UNIT_STEALTH) {
      if (!Unit_IsVisibleToCurrentPlayer(slot)) {
        continue;
      }
    }
    if (!Player_CanSeeUnitAt(MAP_STATE->currentPlayer, unit->x, unit->y)) {
      continue;
    }
    if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
      if (UnitType_BestBaseDamage(UNIT_ANTI_AIR, unit->type) >= 50) {
        continue;
      }
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
      if (counterFirst == TRUE) {
        if (UnitType_BestBaseDamage(UNIT_ANTI_AIR, unit->type) >= 45) {
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
          if (threat > MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)]) {
            MAP_STATE->mapTiles->threatMax[TILE_INDEX(x, y)] = threat;
          }
          if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR)) {
            MAP_STATE->mapTiles->rangeScratch[TILE_INDEX(x, y)] = 1;
          }
        }
      }
    } else {
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

/* Counterpart of Ai_BuildThreatMaps for the end-of-turn phase: same clearing and
 * object threat, but each enemy player is added by AiEndActions_AddEnemyThreat,
 * which rates threat against an Anti-Air instead of the acting unit. The name
 * is a guess from the caller. */
void AiEndActions_BuildThreatMaps(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = MAP_STATE->unk_1e;
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      AiEndActions_AddEnemyThreat(player);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
}
