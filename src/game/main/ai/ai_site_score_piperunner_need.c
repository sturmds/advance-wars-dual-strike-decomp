#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/range_map.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(s8 mark);
int UnitType_GetMaxRange(int unitType);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define TERRAIN_PIPE 15 /* guess: TERRAIN_PIPE_SEAM is 16 */
#define PIPE_MOVE_LIMIT \
  110 /* RangeMap_FillMoves movement; tiles up to it are reachable */

/* End-actions step: rates each base AiSite next to a pipe tile for
 * Piperunners. It fills the Piperunner move and attack range from the site,
 * then counts the units on it: the current player's Piperunners within move
 * range, and armed enemy units (4 for a Piperunner, 1 for any other).
 * score = (enemies + 4) / 8 - own Piperunners. */
void AiSite_ScorePiperunnerNeed(void) {
  /* needed to match: declaration order sets the register choice, and the
     * second scan uses its own row and col */
  int maxY;
  int minX;
  MapUnit* unit;
  AiSite* site;
  int maxX;
  int slot;
  int y;
  int dist;
  int minY;
  u8 found;
  int x;
  int ownCount;
  int i;
  int range;
  int enemyCount;
  int col;
  int row;

  site = data_0229e5bc;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind == SITE_KIND_SKIPPED || site->type != SITE_BASE) {
      continue;
    }
    found = 0;
    minX = site->x - 1;
    if (minX < 0) {
      minX = 0;
    }
    minY = site->y - 1;
    if (minY < 0) {
      minY = 0;
    }
    maxX = STATE->unk_1c4->mapWidth - 1;
    if (site->x + 1 < maxX) {
      maxX = site->x + 1;
    }
    maxY = STATE->unk_1c4->mapHeight - 1;
    if (site->y + 1 < maxY) {
      maxY = site->y + 1;
    }
    for (y = minY; y <= maxY; y++) {
      for (x = minX; x <= maxX; x++) {
        dist = 0;
        if (site->x - x > 0) {
          dist += site->x - x;
        } else {
          dist += x - site->x;
        }
        if (site->y - y > 0) {
          dist += site->y - y;
        } else {
          dist += y - site->y;
        }
        if (dist < 1 || dist > 1) {
          continue;
        }
        if ((u32)(MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]) -
                  TERRAIN_PIPE) <= 1) { /* pipe or pipe seam */
          found = 1;
        }
      }
    }
    if (!found) {
      continue;
    }

    RangeMap_FillMoves(site->x, site->y, UNIT_PIPERUNNER, PIPE_MOVE_LIMIT, 1);
    for (range = 0; range < UnitType_GetMaxRange(UNIT_PIPERUNNER); range++) {
      RangeMap_MarkAdjacentTiles(PIPE_MOVE_LIMIT + 1 + range);
    }

    ownCount = 0;
    enemyCount = 0;
    for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
      for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
        if (STATE->rangeMap[row][col] < 0) {
          continue;
        }
        slot = STATE->mapTiles->unitAt[TILE(col, row)];
        if (slot == 0) {
          continue;
        }
        if (Unit_IsCurrentPlayers(slot) == 1) {
          if (STATE->rangeMap[row][col] > PIPE_MOVE_LIMIT) {
            continue;
          }
          if (STATE->units[slot].type == UNIT_PIPERUNNER) {
            ownCount++;
          }
        } else {
          if (STATE->players[STATE->currentPlayer].team ==
              STATE->players[(slot >> 6) + 1].team) {
            continue;
          }
          unit = &STATE->units[slot];
          if (UnitType_GetMaxRange(unit->type) != 0) {
            if (unit->type == UNIT_PIPERUNNER) {
              enemyCount += 4;
            } else {
              enemyCount += 1;
            }
          }
        }
      }
    }
    site->score = ((enemyCount + 4) >> 3) - ownCount;
  }
}
