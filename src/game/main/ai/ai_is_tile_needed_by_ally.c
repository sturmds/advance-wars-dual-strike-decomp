#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int UnitType_GetAttackKind(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

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
    shoreMaxX = STATE->unk_1c4->mapWidth - 1;
    if (x + 1 < shoreMaxX) {
      shoreMaxX = x + 1;
    }
    shoreMaxY = STATE->unk_1c4->mapHeight - 1;
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
        terrain =
            MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(shoreX, shoreY)]);
        if (terrain == TERRAIN_SHOAL || terrain == TERRAIN_PORT) {
          slot = STATE->mapTiles->unitAt[TILE(shoreX, shoreY)];
          if (slot == 0) {
            continue;
          }
          if (STATE->players[STATE->currentPlayer].team !=
              STATE->players[(slot >> 6) + 1].team) {
            continue;
          }
          if (STATE->units[slot].transportWanted == 2) {
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
  farMaxX = STATE->unk_1c4->mapWidth - 1;
  if (sideMaxX >= farMaxX) {
    sideMaxX = farMaxX;
  }
  sideMaxY = y + 1;
  farMaxY = STATE->unk_1c4->mapHeight - 1;
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
      slot = STATE->mapTiles->unitAt[TILE(sideX, sideY)];
      if (slot == 0) {
        if (AI_TARGET_FLAGS->attackTiles) {
          continue;
        }
        if ((STATE->tileHp[TILE(sideX, sideY)] & 0x7f) == 0) {
          continue;
        }
      }
      if (STATE->players[STATE->currentPlayer].team ==
          STATE->players[(slot >> 6) + 1].team) {
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
      freeMaxX = STATE->unk_1c4->mapWidth - 1;
      if (enemyX + 1 < freeMaxX) {
        freeMaxX = enemyX + 1;
      }
      freeMaxY = STATE->unk_1c4->mapHeight - 1;
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
          if (STATE->rangeMap[freeY][freeX] < 0) {
            continue;
          }
          if (STATE->mapTiles->unitAt[TILE(freeX, freeY)] == 0) {
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
      slot = STATE->mapTiles->unitAt[TILE(farX, farY)];
      if (slot == 0) {
        continue;
      }
      if (farX == STATE->actingUnit->x && farY == STATE->actingUnit->y) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      unit = &STATE->units[slot];
      if (unit->flags & MAP_UNIT_WAITED) {
        continue;
      }
      if (!(x == STATE->actingUnit->x && y == STATE->actingUnit->y) &&
          unit->unk_08_7) {
        continue;
      }
      if (UnitType_GetAttackKind(unit->type) != ATTACK_KIND_DIRECT) {
        continue;
      }
      if (Ov000_gUnitStats[STATE->actingUnit->type].moveGroup &
          (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
        if (!(Ov000_gUnitStats[unit->type].moveGroup &
              (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
          continue;
        }
      } else if (Ov000_gUnitStats[STATE->actingUnit->type].moveGroup &
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
