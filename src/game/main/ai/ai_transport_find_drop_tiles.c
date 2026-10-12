#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int MainLoop_GetFrameCount(void);
int AiTransport_ScoreDropTile(int x, int y, MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define NO_TILE 9999

/* Picks where the acting transport, standing at (x, y), unloads its cargo:
 * of the four tiles next to it, the one AiTransport_ScoreDropTile scores highest for
 * each cargo unit (a coin flip on the frame count breaks ties; the second
 * unit cannot take the first one's tile). Returns how many units got a
 * tile: 0, 1 or 2. */
int AiTransport_FindDropTiles(int x, int y, s16* firstDrop, s16* secondDrop) {
  /* declaration order and the shared cargo, score and best needed to
     * match (stack slots and registers); the "2" locals serve the second
     * cargo unit */
  int maxY2;
  int ty2;
  int maxX2;
  int minY2;
  int minX2;
  int tx2;
  int ty;
  int dist2;
  int maxY;
  int minY;
  int maxX;
  int best;
  int score;
  int tx;
  int dist;
  int minX;
  MapUnit* cargo;

  if (STATE->actingUnit->cargo[0] == 0) {
    return 0;
  }
  cargo = &STATE->units[STATE->actingUnit->cargo[0]];
  best = 0;
  firstDrop[0] = NO_TILE;
  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
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
      if (dist < 1 || dist > 1) {
        continue;
      }
      score = AiTransport_ScoreDropTile(tx, ty, cargo);
      if (score < best) {
        continue;
      }
      if (score == best && (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      best = score;
      firstDrop[0] = tx;
      firstDrop[1] = ty;
    }
  }
  if (firstDrop[0] == NO_TILE) {
    return 0;
  }

  if (STATE->actingUnit->cargo[1] == 0) {
    return 1;
  }
  cargo = &STATE->units[STATE->actingUnit->cargo[1]];
  best = 0;
  secondDrop[0] = NO_TILE;
  minX2 = x - 1;
  if (minX2 < 0) {
    minX2 = 0;
  }
  minY2 = y - 1;
  if (minY2 < 0) {
    minY2 = 0;
  }
  maxX2 = STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX2) {
    maxX2 = x + 1;
  }
  maxY2 = STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY2) {
    maxY2 = y + 1;
  }
  for (ty2 = minY2; ty2 <= maxY2; ty2++) {
    for (tx2 = minX2; tx2 <= maxX2; tx2++) {
      dist2 = 0;
      if (x - tx2 > 0) {
        dist2 += x - tx2;
      } else {
        dist2 += tx2 - x;
      }
      if (y - ty2 > 0) {
        dist2 += y - ty2;
      } else {
        dist2 += ty2 - y;
      }
      if (dist2 < 1 || dist2 > 1) {
        continue;
      }
      if (tx2 == firstDrop[0] && ty2 == firstDrop[1]) {
        continue;
      }
      score = AiTransport_ScoreDropTile(tx2, ty2, cargo);
      if (score < best) {
        continue;
      }
      if (score == best && (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      best = score;
      secondDrop[0] = tx2;
      secondDrop[1] = ty2;
    }
  }
  return secondDrop[0] == NO_TILE ? 1 : 2;
}
