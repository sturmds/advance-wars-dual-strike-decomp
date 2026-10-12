#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiBlackBomb_GetStartScore(void);
int AiBlackBomb_ScoreExplosionAt(int x, int y);
void RangeMap_FillUnitMoves(MapUnit* unit);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Scores every empty tile the acting Black Bomb can reach with
 * AiBlackBomb_ScoreExplosionAt and acts on the last tile that scores at least as much
 * as the start value: AiBlackBomb_GetStartScore, or 1 when the bomb has under 10
 * fuel. Action 23 is the explosion (guess from the caller). */
void AiBlackBomb_ChooseExplosionTile(void) {
  int bestX;
  int bestY;
  int bestScore;
  int score;
  int x;
  int y;

  bestX = -1;
  bestY = bestX;
  bestScore = AiBlackBomb_GetStartScore();
  if (STATE->actingUnit->fuel < 10) {
    bestScore = 1;
  }
  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
        continue;
      }
      score = AiBlackBomb_ScoreExplosionAt(x, y);
      if (score >= bestScore) {
        bestX = x;
        bestY = y;
        bestScore = score;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_PerformAction(bestX, bestY, 23, 0, 0);
}
