#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s8 data_027c074c;
extern int data_027c0750;
extern MoveSearchNode* data_027c0754;
extern MoveSearch* data_027c0758;
extern MoveSearchNode* data_027c075c;

int Player_GetUnitMoveRange(int player, int unitType);
void MoveSearch_LoadMoveCosts(int unitType);
void MoveSearch_TryStepOverTurns(
    int direction); /* queues the tile next to the current one in that direction */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define FROM_START 1
#define DIR_UP 2
#define DIR_DOWN 3
#define DIR_LEFT 4
#define DIR_RIGHT 5

/* Fills rangeMap with the move cost from (x, y) for a unit type, as
 * RangeMap_FillMoves does, but a path may go on past the unit's movement
 * for several turns: MapTiles.rangeScratch gets the number of turns each
 * tile takes and rangeMap the cost within its last turn. maxCost limits
 * the search. Both maps hold -1 for tiles not reached. */
void RangeMap_FillMovesOverTurns(int x, int y, int unitType, int maxCost) {
  RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
  RangeMap_Fill(-1);
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
  RangeMap_Fill(-1);
  data_027c0750 = 0;
  data_027c074c = 0;
  data_027c075c = data_027c0758->lists[0];
  data_027c0754 = data_027c0758->lists[1];
  data_027c0758->maxCost = maxCost;
  data_027c0758->moveRange =
      Player_GetUnitMoveRange(STATE->currentPlayer, unitType);
  MoveSearch_LoadMoveCosts(unitType);
  data_027c075c->x = x;
  data_027c075c->y = y;
  data_027c075c->from = FROM_START;
  data_027c075c->cost = 0;
  STATE->rangeMap[y][x] = 0;
  STATE->mapTiles->rangeScratch[TILE(x, y)] = 0;
  data_027c075c++;
  data_027c075c->from = 0;
  for (;;) {
    /* swap the lists: expand what was found, collect into the other */
    if (data_027c074c == 0) {
      data_027c074c = 1;
      data_027c075c = data_027c0758->lists[1];
      data_027c0754 = data_027c0758->lists[0];
      data_027c0750 = 0;
    } else {
      data_027c074c = 0;
      data_027c075c = data_027c0758->lists[0];
      data_027c0754 = data_027c0758->lists[1];
      data_027c0750 = 0;
    }
    if (data_027c0754->from == 0) {
      break;
    }
    while (data_027c0754->from != 0) {
      switch (data_027c0754->from) {
        case FROM_START:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
        case DIR_UP:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
        case DIR_DOWN:
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
        case DIR_LEFT:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          break;
        case DIR_RIGHT:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
      }
      data_027c075c->from = 0;
      data_027c0754++;
    }
  }
  data_027c0758->x = STATE->unk_1c4->mapWidth;
  data_027c0758->y = STATE->unk_1c4->mapHeight;
}
