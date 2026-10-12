#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
int AiTransport_CountUnloadTiles(int x, int y);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff

/* Run by func_020b5bf4 after AiTransport_UnloadInReach, which unloads the acting
 * transport on a tile in reach when it can. This one looks past this turn:
 * among the tiles with a nonzero MapTiles.rangeScratch2 value (a score
 * from code not read yet) that AiTransport_CountUnloadTiles accepts and that lie beyond
 * one turn's move (gAiTurnState->unk_05, likely the move per turn), it
 * takes the fewest turns away, then the highest score, and heads there
 * with AiUnit_MoveTowardTile. */
void AiTransport_HeadForUnloadTile(void) {
  /* declaration order needed to match (register allocation) */
  int bestTurns;
  int bestX;
  int bestValue;
  int value;
  int x;
  int y;
  int bestY;
  int turns;

  bestX = -1;
  bestY = bestX;
  bestTurns = NO_TILE;
  bestValue = 0;
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (STATE->rangeMap[y][x] <= gAiTurnState->unk_05) {
        continue;
      }
      if (STATE->mapTiles->rangeScratch2[TILE(x, y)] == 0) {
        continue;
      }
      if (!AiTransport_CountUnloadTiles(x, y)) {
        continue;
      }
      turns = HwDiv32(STATE->rangeMap[y][x], gAiTurnState->unk_05) + 1;
      if (HwMod32(STATE->rangeMap[y][x], gAiTurnState->unk_05) == 0) {
        turns--;
      }
      if (bestTurns < turns) {
        continue;
      }
      value = (u8)STATE->mapTiles->rangeScratch2[TILE(x, y)];
      if (bestTurns == turns && bestValue > value) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestTurns = turns;
      bestValue = value;
    }
  }
  if (bestValue == 0) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}
