#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMaxRange(int player, int unitType);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiAttack_Score(u32 flags, s16* pos);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define MAX_RANGE() \
  GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type)

/* For an indirect unit about to wait at (x, y): the unit slot of the best
 * enemy unit in its fire range from there (at or beyond the unit type's
 * minimum range, within its maximum range with CO bonuses), or 0 when none
 * is visible and safe to hit. Targets are scored by AiAttack_Score with
 * AI_ATTACK_SAFE. */
int AiUnit_PickIndirectTarget(int x, int y) {
  /* needed to match: declaration order and the u8 slots */
  s16 pos[2];
  u8 bestSlot;
  int minX;
  int maxX;
  int maxY;
  u8 slot;
  int bestScore;
  int minY;
  int cy;
  int cx;
  int score;
  int distance;

  bestSlot = 0;
  bestScore = 0;
  minX = (x - MAX_RANGE() < 0) ? 0 : x - MAX_RANGE();
  minY = (y - MAX_RANGE() < 0) ? 0 : y - MAX_RANGE();
  maxX = STATE_IDX_FIRST->unk_1c4->mapWidth - 1;
  if (x + MAX_RANGE() < maxX) {
    maxX = x + MAX_RANGE();
  }
  maxY = STATE_IDX_FIRST->unk_1c4->mapHeight - 1;
  if (y + MAX_RANGE() < maxY) {
    maxY = y + MAX_RANGE();
  }
  for (cy = minY; cy <= maxY; cy++) {
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
      if (distance < Ov000_gUnitStats[STATE->actingUnit->type].minRange ||
          distance > MAX_RANGE()) {
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
