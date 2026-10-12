#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Map_IsTileVisible(s16 x, s16 y);
int AiAttack_Score(int target, s16* pos);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Unit slot of the visible enemy unit next to (x, y) that AiAttack_Score
 * rates above 0 and highest for an attack from (x, y), 0 when there is none. */
int AiUnit_FindBestAdjacentTarget(int x, int y) {
  int maxY;
  int maxX;
  int minX;
  int cy;
  int bestSlot;
  int slot;
  int bestScore;
  int score;
  int cx;
  int dist;
  int minY;
  s16 pos[2];

  bestSlot = 0;
  bestScore = 0;
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
  for (cy = minY; cy <= maxY; cy++) {
    for (cx = minX; cx <= maxX; cx++) {
      dist = 0;
      if (x - cx > 0) {
        dist += x - cx;
      } else {
        dist += cx - x;
      }
      if (y - cy > 0) {
        dist += y - cy;
      } else {
        dist += cy - y;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      slot = STATE->mapTiles->unitAt[TILE(cx, cy)];
      if (slot == 0) {
        continue;
      }
      if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team ==
          STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      if (!Map_IsTileVisible(cx, cy)) {
        continue;
      }
      pos[0] = x;
      pos[1] = y;
      score = AiAttack_Score(slot | AI_ATTACK_SAFE, pos);
      if (bestScore < score) {
        bestSlot = slot;
        bestScore = score;
      }
    }
  }
  return bestSlot;
}
