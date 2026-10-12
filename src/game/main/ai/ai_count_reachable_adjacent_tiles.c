#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Number of tiles next to (x, y) that the acting unit can reach: the range
 * map holds a move cost there that is not above the unit's usable movement
 * (gAiTurnState->unk_05). */
int Ai_CountReachableAdjacentTiles(int x, int y) {
  /* needed to match: declaration order sets registers and stack slots */
  int maxX;
  int maxY;
  int cy;
  int count;
  int minX;
  int cx;
  int distance;
  int cost;

  count = 0;
  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  cy = y - 1;
  if (cy < 0) {
    cy = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
  }
  for (; cy <= maxY; cy++) {
    for (cx = minX; cx <= maxX; cx++) {
      distance = 0;
      if (x - cx > 0) {
        distance += x - cx;
      } else {
        distance += cx - x;
      }
      if (y - cy > 0) {
        distance += y - cy;
      } else {
        distance += cy - y;
      }
      if (distance < 1 || distance > 1) {
        continue;
      }
      cost = STATE->rangeMap[cy][cx];
      if (cost < 0) {
        continue;
      }
      if (cost <= gAiTurnState->unk_05) {
        count++;
      }
    }
  }
  return count;
}
