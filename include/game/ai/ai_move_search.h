#ifndef GAME_AI_AI_MOVE_SEARCH_H
#define GAME_AI_AI_MOVE_SEARCH_H

#include "types.h"

BOOL MoveSearch_FindTileByMechSteps(int x, int y, int unitType, int maxCost);
BOOL MoveSearch_TryMechStep(int direction);
BOOL MoveSearch_FindTile(int x, int y, int unitType, int maxCost);
BOOL MoveSearch_TryStep(int direction);
BOOL func_020a5cc8(int x, int y, int unitType, int maxCost);
BOOL MoveSearch_TryStepInCost(int direction);
int Ai_IsFreeCaptureSiteTile(int x, int y, int arg2);
int Ai_CanDamageEnemyAtTile(int x, int y, int unitType);
int Tile_HasEnemyUnit(int x, int y, int arg2);
int AiTile_GetLowestAdjacentReach(int x, int y);
BOOL AiUnit_HasFuelForTile(s16 x, s16 y, int movesPerTurn);
void RangeMap_FillMovesOverTurns(int x, int y, int unitType, int maxCost);
void MoveSearch_TryStepOverTurns(int direction);

#endif
