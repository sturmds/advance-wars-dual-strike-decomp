#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int MainLoop_GetFrameCount(void); /* random number */
int AiTile_ScoreAttackFrom(
    int x, int y); /* score of (x, y) as a tile to attack from; -1: unusable */

#define STATE (data_027c027c->unk_48[data_027c0284])

#define NO_TILE 9999

/* Picks the tile next to (targetX, targetY) that AiTile_ScoreAttackFrom scores
 * highest for the acting unit to attack from, and writes it to pos. Ties
 * go to a coin flip. FALSE: every side scores below 0. */
BOOL FindAttackTile(int targetX, int targetY, s16* pos) {
  int maxY;
  int maxX;
  int minX;
  int y;
  int bestScore;
  int score;
  int x;
  int dist;
  int minY;

  pos[0] = NO_TILE;
  bestScore = 0;
  minX = targetX - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = targetY - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (targetX + 1 < maxX) {
    maxX = targetX + 1;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (targetY + 1 < maxY) {
    maxY = targetY + 1;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (targetX - x > 0) {
        dist += targetX - x;
      } else {
        dist += x - targetX;
      }
      if (targetY - y > 0) {
        dist += targetY - y;
      } else {
        dist += y - targetY;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      score = AiTile_ScoreAttackFrom(x, y);
      if (score < bestScore) {
        continue;
      }
      if (score == bestScore && (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      pos[0] = x;
      bestScore = score;
      pos[1] = y;
    }
  }
  return pos[0] != NO_TILE;
}
