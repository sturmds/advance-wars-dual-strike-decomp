#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int UnitType_GetAttackKind(int unitType);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Ai_UnitUsesThreatMaps(int unitType);
void Ai_AddOoziumThreat(MapUnit* unit);
int Ai_RatedBaseDamage(int unitType, int targetType);
BOOL Ai_EnemyWithinReach(MapUnit* acting, MapUnit* enemy, int arg2);
BOOL Player_CanSeeUnitAt(u16 player, s16 x, s16 y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
int Player_GetRepairHp(int playerIndex, int slot, int powerState);
int GetUnitBaseMaxRange(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define ACTING (STATE->actingUnit)
#define TILE_INDEX(x, y) (STATE->mapTiles->rowStart[y] + (x))
#define OWNER(unit) MAP_UNIT_OWNER(STATE, unit)
#define MAX_RANGE(unit) GetUnitBaseMaxRange(OWNER(unit), (unit)->type)

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
        /* tiles that fail Player_CanSeeUnitAt count only against air units */
        if (Player_CanSeeUnitAt(STATE->unk_1e, targetX, targetY) ||
            (Ov000_gUnitStats[ACTING->type].moveGroup & MOVE_GROUP_AIR)) {
          damage =
              STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] + threat;
          if (damage > 255) {
            damage = 255;
          }
          STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] = damage;
        }
      }
    }
  }
}
