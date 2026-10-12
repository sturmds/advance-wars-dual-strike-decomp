#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int value);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define UNLIMITED_MOVE 110
#define CRUISER_GUARD_HP 40

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

  STATE->unk_1e = player;
  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    unit = &STATE->units[slot];
    if (unit->type == UNIT_NONE || unit->type != UNIT_SUB || unit->hp <= 20) {
      continue;
    }
    RangeMap_FillMoves(unit->x, unit->y, unit->type, UNLIMITED_MOVE, 0);
    RangeMap_MarkAdjacentTiles(111);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
        if (STATE->rangeMap[y][x] < 0) {
          continue;
        }
        if (Unit_IsCurrentPlayers(STATE->mapTiles->unk_508[TILE(x, y)]) == 0) {
          continue;
        }
        other = &STATE->units[STATE->mapTiles->unitAt[TILE(x, y)]];
        if (other->type == UNIT_CRUISER && other->hp >= CRUISER_GUARD_HP) {
          goto next;
        }
      }
    }
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
      for (fillX = 0; fillX < STATE->unk_1c4->mapWidth; fillX++) {
        if (STATE->rangeMap[y][fillX] < 0) {
          continue;
        }
        if (STATE->rangeMap[y][fillX] <=
            STATE->mapTiles->rangeScratch[TILE(fillX, y)]) {
          STATE->mapTiles->rangeScratch[TILE(fillX, y)] =
              STATE->rangeMap[y][fillX];
        }
      }
    }
  next:;
  }
}
