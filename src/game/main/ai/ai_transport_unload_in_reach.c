#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
int AiTransport_CountUnloadTiles(int x, int y);
int MainLoop_GetFrameCount(void);
void AiTransport_UnloadAt(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define NO_TILE 0x7fff

/* Added to a tile's score when the transport carries two units and both
 * can get off there. */
#define BOTH_CARGO_BONUS 0x100

/* Run first by func_020b5bf4: drops the acting transport's cargo this
 * turn if it can. Of the free tiles in reach that have a nonzero
 * MapTiles.rangeScratch2 score and room to unload, takes the highest
 * score, then the lower threatMax + 2 * threatSum, and on a full tie
 * keeps the old tile on odd frames. Does nothing when no tile qualifies. */
void AiTransport_UnloadInReach(void) {
  /* declaration order needed to match (register allocation) */
  int count;
  int bestScore;
  int bestThreat;
  int score;
  int threat;
  int bestX;
  int bestY;
  int x;
  int y;

  bestX = -1;
  bestY = bestX;
  bestScore = 0;
  bestThreat = NO_TILE;
  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
        continue;
      }
      if (STATE->mapTiles->rangeScratch2[TILE(x, y)] == 0) {
        continue;
      }
      count = AiTransport_CountUnloadTiles(x, y);
      if (count == 0) {
        continue;
      }
      score = (u8)STATE->mapTiles->rangeScratch2[TILE(x, y)];
      if (STATE->actingUnit->cargo[1] != 0 && count >= 2) {
        score |= BOTH_CARGO_BONUS;
      }
      if (bestScore > score) {
        continue;
      }
      threat = STATE->mapTiles->threatMax[TILE(x, y)] +
               STATE->mapTiles->threatSum[TILE(x, y)] * 2;
      if (bestScore == score && bestThreat < threat) {
        continue;
      }
      if (bestScore == score && bestThreat == threat &&
          (MainLoop_GetFrameCount() & 1)) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestScore = score;
      bestThreat = threat;
    }
  }
  if (bestScore == 0) {
    return;
  }
  AiTransport_UnloadAt(bestX, bestY);
}
