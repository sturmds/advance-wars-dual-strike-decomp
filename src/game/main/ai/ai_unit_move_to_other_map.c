#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ai_unit_action.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void RangeMap_FillUnitMoves(MapUnit* unit);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
BOOL AiUnit_CanUseOtherMapFrom(MapUnit* unit, s16 x, s16 y);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern Unk_02080440_48 data_02189cd8;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* The game state of the other map, one of the two in data_027c0288. */
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

/* Name is a guess from the checks. In a two-map battle, when the other
 * map's state has unk_30 clear and the acting unit may use it (claimedLandmass
 * clear), moves the unit to the reachable tile with the lowest rangeMap
 * value that AiUnit_CanUseOtherMapFrom accepts, with action 26. */
void AiUnit_MoveToOtherMap(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u32 otherUnk30;
  s16 bestX;
  s16 bestY;
  int bestReach = 0x7fff;
  int x;
  int y;

  if (GAME->gameMode == GAME_MODE_CAMPAIGN) {
    return;
  }
  if (GAME->secondMapId == 0) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(OTHER_MAP);
  saved027c = data_027c027c;
  Screen_SetCurrent(OTHER_MAP);
  otherUnk30 = GAME->unk_1c4->unk_30;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (otherUnk30 != 0) {
    return;
  }
  if (GAME == &data_02189cd8) {
    return;
  }
  if (GAME->actingUnit->claimedLandmass) {
    return;
  }
  if (gAiTurnState->factoryDomainsOwn == 0) {
    return;
  }
  RangeMap_FillUnitMoves(GAME->actingUnit);
  bestX = -1;
  for (y = 0; y < GAME->unk_1c4->mapHeight; y++) {
    for (x = 0; x < GAME->unk_1c4->mapWidth; x++) {
      if (GAME->rangeMap[y][x] < 0) {
        continue;
      }
      if (AiUnit_CanUseOtherMapFrom(GAME->actingUnit, x, y) != 1) {
        continue;
      }
      if (GAME->rangeMap[y][x] <= bestReach) {
        bestY = y;
        bestReach = GAME->rangeMap[y][x];
        bestX = x;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_PerformAction(bestX, bestY, 26, 0, 0);
}
