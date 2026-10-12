#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int MainLoop_GetFrameCount(void); /* random number */
int Ai_IsTransportStopTile(int unitType, int x, int y);
int AiTile_ScoreAttackFrom(
    int x, int y); /* score of (x, y) as a tile to attack from; -1: unusable */

#define STATE (data_027c027c->unk_48[data_027c0284])

#define NO_TILE 9999

/* Name is a guess. Like FindAttackTile, but a side must also pass Ai_IsTransportStopTile for the
 * acting unit's type. Writes the best side to pos; ties go to a coin flip.
 * FALSE: no side qualifies. */
BOOL AiUnit_FindStandTileNextTo(int targetX, int targetY, s16* pos) {
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
      if (!Ai_IsTransportStopTile(STATE->actingUnit->type, x, y)) {
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
