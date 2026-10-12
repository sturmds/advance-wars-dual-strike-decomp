#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
BOOL CounterFirst(int player);
int UnitType_GetAttackKind(int unitType);
int UnitType_BestBaseDamage(int unitType, int targetType);
int Player_GetUnitMoveRange(int player, int unitType);
/* movement is s16 in fill_unit_moves.c; int here, needed to match (no sign extension) */
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Ai_UnitUsesThreatMaps(int unitType);
void Ai_AddOoziumThreat(MapUnit* unit);
int Ai_RatedBaseDamage(
    int unitType,
    int targetType); /* best base damage, scaled for Fighters vs Fighters and Anti-Air vs Fighters */
BOOL Ai_EnemyWithinReach(MapUnit* acting, MapUnit* enemy, int arg2);
BOOL Ai_BestDamageVsDived(int enemyType, int actingType, int arg2);
BOOL Player_CanSeeUnitAt(u16 player, s16 x,
                         s16 y); /* fog, weather and terrain check of a tile */
BOOL Unit_IsBlackHoleOnFlaggedTile(
    MapUnit* unit, int x,
    int y); /* Black Hole unit on a tile with tileHp bit 7 */
int Player_GetRepairHp(int playerIndex, int slot, int powerState);
int GetUnitBaseMaxRange(int player,
                        int unitType); /* max range with no power on */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define ACTING (STATE->actingUnit)
#define TILE_INDEX(x, y) (STATE->mapTiles->rowStart[y] + (x))
#define OWNER(unit) MAP_UNIT_OWNER(STATE, unit)
#define MAX_RANGE(unit) GetUnitBaseMaxRange(OWNER(unit), (unit)->type)

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
    counterFirst =
        CounterFirst((u8)STATE->currentPlayer); /* needed to match: the cast */
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
  /* 50 unit slots per player */
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
    if (!Player_CanSeeUnitAt(STATE->currentPlayer, unit->x, unit->y)) {
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
      for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
          if (STATE->rangeMap[y][x] < 0) {
            continue;
          }
          /* tiles that fail Player_CanSeeUnitAt count only against air units */
          if (Player_CanSeeUnitAt(STATE->unk_1e, x, y) ||
              (Ov000_gUnitStats[ACTING->type].moveGroup & MOVE_GROUP_AIR)) {
            if (threat > STATE->mapTiles->threatMax[TILE_INDEX(x, y)]) {
              STATE->mapTiles->threatMax[TILE_INDEX(x, y)] = threat;
            }
          }
        }
      }
    } else {
      /* ground units ignore indirect attackers they cannot see */
      if (STATE->mapTiles->inSight[TILE_INDEX(unit->x, unit->y)] == 0 &&
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
          if (Player_CanSeeUnitAt(STATE->unk_1e, targetX, targetY) ||
              (Ov000_gUnitStats[ACTING->type].moveGroup & MOVE_GROUP_AIR)) {
            /* needed to match: the sum is kept in damage */
            damage = STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] +
                     threat;
            if (damage > 255) {
              damage = 255;
            }
            STATE->mapTiles->threatSum[TILE_INDEX(targetX, targetY)] = damage;
          }
        }
      }
    }
  }
}
