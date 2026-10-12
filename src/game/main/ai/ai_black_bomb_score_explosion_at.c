#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiBlackBomb_ScoreUnit(MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

/* Radius of a Black Bomb explosion in tiles. */
#define BLOMB_RADIUS 3

/* Adds up AiBlackBomb_ScoreUnit over the units within 3 tiles (by taxicab
 * distance, not on the centre tile) of (x, y). Skipped: Black Bombs,
 * Oozium, unarmed units, units at 10 hp or less and tiles out of sight. */
int AiBlackBomb_ScoreExplosionAt(int x, int y) {
  /* needed to match: declaration order sets the register choice */
  int maxY;
  int dist;
  int ty;
  MapUnit* unit;
  int maxX;
  int total;
  int minX;
  int tx;
  int minY;

  total = 0;
  minX = x - BLOMB_RADIUS;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - BLOMB_RADIUS;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + BLOMB_RADIUS < maxX) {
    maxX = x + BLOMB_RADIUS;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (y + BLOMB_RADIUS < maxY) {
    maxY = y + BLOMB_RADIUS;
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
      if (dist < 1 || dist > BLOMB_RADIUS) {
        continue;
      }
      unit = &STATE->units[MAP->unitAt[TILE_INDEX(tx, ty)]];
      if (unit->type == UNIT_NONE || unit->type == UNIT_OOZIUM ||
          unit->type == UNIT_BLACK_BOMB) {
        continue;
      }
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_NONE) {
        continue;
      }
      if (unit->hp <= 10) {
        continue;
      }
      if (!Map_IsTileVisible(unit->x, unit->y)) {
        continue;
      }
      total += AiBlackBomb_ScoreUnit(unit);
    }
  }
  return total;
}
