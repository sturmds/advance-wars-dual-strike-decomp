#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Player_GetUnitMoveRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define OWNER(unit) MAP_UNIT_OWNER(STATE, unit)

/* For each Sub of the player: fills rangeMap with the tiles it can reach or
 * stand next to, then adds 1 to MapTiles.rangeScratch on each of those
 * tiles. A Sub with a Cruiser of the current player's team in that area is
 * skipped. Ai_CountEnemySubReach calls it for each enemy player. */
void Ai_AddSubReach(u8 player) {
  /* needed to match: the second scan has its own x and y */
  int slot;
  MapUnit* unit;
  MapUnit* other;
  int x;
  int y;
  int markX;
  int markY;

  STATE->unk_1e = player;
  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    unit = &STATE->units[slot];
    /* needed to match: the UNIT_NONE test, though the Sub test covers it */
    if (unit->type == UNIT_NONE || unit->type != UNIT_SUB) {
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
        other = &STATE->units[STATE->mapTiles->unitAt[TILE_INDEX(x, y)]];
        if (STATE->players[STATE->currentPlayer].team ==
                STATE->players[OWNER(other)].team &&
            other->type == UNIT_CRUISER) {
          goto nextSub;
        }
      }
    }
    for (markY = 0; markY < STATE->unk_1c4->mapHeight; markY++) {
      for (markX = 0; markX < STATE->unk_1c4->mapWidth; markX++) {
        if (STATE->rangeMap[markY][markX] >= 0) {
          STATE->mapTiles->rangeScratch[TILE_INDEX(markX, markY)]++;
        }
      }
    }
  nextSub:;
  }
}
