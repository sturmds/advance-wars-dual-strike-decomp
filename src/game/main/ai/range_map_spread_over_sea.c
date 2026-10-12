#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL Lander_CanEnterTile(int tile);

/* One step of a flood fill in rangeMap: every marked tile (not negative)
 * whose mark differs from `mark` and that a Lander can sail on
 * (Lander_CanEnterTile) gives `mark` to each unmarked neighbour. AiTurn_TryBuildLanderNearSite
 * passes marks from 111 up. */
void RangeMap_SpreadOverSea(u8 mark) {
  int x;
  int y;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0 || mark == STATE->rangeMap[y][x]) {
        continue;
      }
      if (!Lander_CanEnterTile(
              STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]])) {
        continue;
      }
      if (x != 0 && STATE->rangeMap[y][x - 1] < 0) {
        STATE->rangeMap[y][x - 1] = mark;
      }
      if (x != STATE->unk_1c4->mapWidth - 1 && STATE->rangeMap[y][x + 1] < 0) {
        STATE->rangeMap[y][x + 1] = mark;
      }
      if (y != 0 && STATE->rangeMap[y - 1][x] < 0) {
        STATE->rangeMap[y - 1][x] = mark;
      }
      if (y != STATE->unk_1c4->mapHeight - 1 && STATE->rangeMap[y + 1][x] < 0) {
        STATE->rangeMap[y + 1][x] = mark;
      }
    }
  }
}
