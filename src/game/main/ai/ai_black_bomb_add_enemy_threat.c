#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int UnitType_GetAttackKind(int unitType);
int Player_GetUnitMoveRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Ai_UnitUsesThreatMaps(int unitType);
void Ai_AddOoziumThreat(MapUnit* unit);
int Ai_RatedBaseDamage(int unitType, int targetType);
BOOL Ai_EnemyWithinReach(MapUnit* acting, MapUnit* enemy, int arg2);
BOOL Ai_BestDamageVsDived(int enemyType, int actingType, int arg2);
BOOL Player_CanSeeUnitAt(u16 player, s16 x, s16 y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
int Player_GetRepairHp(int playerIndex, int slot, int powerState);
int GetUnitBaseMaxRange(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define ACTING (STATE->actingUnit)
#define TILE_INDEX(x, y) (STATE->mapTiles->rowStart[y] + (x))
#define OWNER(unit) MAP_UNIT_OWNER(STATE, unit)
#define MAX_RANGE(unit) GetUnitBaseMaxRange(OWNER(unit), (unit)->type)

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
  if (STATE->players[STATE->currentPlayer].aiBehaviour ==
      AI_BEHAVIOUR_ASSAULT) {
    if (UnitType_GetAttackKind(ACTING->type) == ATTACK_KIND_DIRECT ||
        UnitType_GetAttackKind(ACTING->type) == ATTACK_KIND_NONE) {
      skipDirectEnemies = TRUE;
    }
  }
  STATE->unk_1e = player;
  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    unit = &STATE->units[slot];
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
    if (!Player_CanSeeUnitAt(STATE->currentPlayer, unit->x, unit->y)) {
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
      for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
          if (STATE->rangeMap[y][x] < 0) {
            continue;
          }
          if (threat > STATE->mapTiles->threatMax[TILE_INDEX(x, y)]) {
            STATE->mapTiles->threatMax[TILE_INDEX(x, y)] = threat;
          }
          if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR)) {
            STATE->mapTiles->rangeScratch[TILE_INDEX(x, y)] = 1;
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
      maxX = unit->x + MAX_RANGE(unit) < STATE->unk_1c4->mapWidth - 1
                 ? unit->x + MAX_RANGE(unit)
                 : STATE->unk_1c4->mapWidth - 1;
      maxY = unit->y + MAX_RANGE(unit) < STATE->unk_1c4->mapHeight - 1
                 ? unit->y + MAX_RANGE(unit)
                 : STATE->unk_1c4->mapHeight - 1;
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
              STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] + threat;
          if (damage > 255) {
            damage = 255;
          }
          STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] = damage;
          if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR)) {
            STATE->mapTiles->rangeScratch[TILE_INDEX(targetX, targetY)] = 1;
          }
        }
      }
    }
  }
}
