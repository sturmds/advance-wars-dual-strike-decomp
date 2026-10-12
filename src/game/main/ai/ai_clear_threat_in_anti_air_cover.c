#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_BestBaseDamage(int unitType, int targetType);
int UnitType_GetAttackKind(int unitType);
void RangeMap_FillUnitMoves(MapUnit* unit);
int GetUnitBaseMaxRange(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE(x, y) ((x) + MAP->rowStart[y])
#define RANGE(unit) GetUnitBaseMaxRange(STATE->currentPlayer, (unit)->type)

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

  for (slot = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; slot++) {
    unit = &STATE->units[slot];
    if (unit->type == 0 ||
        UnitType_BestBaseDamage(unit->type, UNIT_FIGHTER) == 0) {
      continue;
    }
    if ((unit->flags & MAP_UNIT_CARRIED) || unit->hp < 61 || unit->ammo == 0) {
      continue;
    }

    if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
      RangeMap_FillUnitMoves(unit);
      for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
          if (STATE->rangeMap[y][x] >= 0 &&
              MAP->rangeScratch[TILE(x, y)] == 0) {
            MAP->threatMax[TILE(x, y)] = 0;
            MAP->threatSum[TILE(x, y)] = 0;
          }
        }
      }
    } else {
      minX = unit->x - RANGE(unit) - 1 < 0 ? 0 : unit->x - RANGE(unit) - 1;
      minY = unit->y - RANGE(unit) - 1 < 0 ? 0 : unit->y - RANGE(unit) - 1;
      maxX = unit->x + RANGE(unit) - 1 < STATE->unk_1c4->mapWidth - 1
                 ? unit->x + RANGE(unit) - 1
                 : STATE->unk_1c4->mapWidth - 1;
      maxY = unit->y + RANGE(unit) - 1 < STATE->unk_1c4->mapHeight - 1
                 ? unit->y + RANGE(unit) - 1
                 : STATE->unk_1c4->mapHeight - 1;
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
          if (dist < Ov000_gUnitStats[STATE->actingUnit->type].minRange + 1 ||
              dist > RANGE(unit) - 1) {
            continue;
          }
          if (MAP->rangeScratch[TILE(tx, ty)] == 0) {
            MAP->threatMax[TILE(tx, ty)] = 0;
            MAP->threatSum[TILE(tx, ty)] = 0;
          }
        }
      }
    }
  }
}
