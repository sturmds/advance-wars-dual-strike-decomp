#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Clears the range map tiles next to (x, y) in the directions set in
 * dirs: 1 up, 2 down, 4 left, 8 right. */
void RangeMap_ClearAdjacent(int x, int y, int dirs) {
  if (dirs & 4) {
    STATE->rangeMap[y][x - 1] = 0;
  }
  if (dirs & 8) {
    STATE->rangeMap[y][x + 1] = 0;
  }
  if (dirs & 1) {
    STATE->rangeMap[y - 1][x] = 0;
  }
  if (dirs & 2) {
    STATE->rangeMap[y + 1][x] = 0;
  }
}
